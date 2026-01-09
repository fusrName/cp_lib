#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
目的:
 - g++ -MM を Python から呼んで依存リスト（stdout）を取得する。
 - 依存に載っているユーザヘッダだけを再帰的に展開し、元ソースの #include 行を置換した
   結果を標準出力に出す。
"""

# --- 標準ライブラリのインポート ---
# argparse: コマンドライン引数を解析するためのモジュール
# subprocess: 外部コマンド（ここでは g++）を呼び出すために使う
# shlex: シェル風の文字列分割（例: "-I include -DNAME" を安全に分割）
# re: 正規表現（テキストのパターン検索・抽出）
# os, sys: ファイル操作やプロセス終了、標準入出力操作
# io: メモリ上のテキストストリーム（stdin 文字列を file-like に扱うのに利用）
# itertools: イテレータユーティリティ（複数リストを連結する等）
import argparse
import subprocess
import shlex
import re
import os
import sys
import io
import itertools

# 正規表現: #include "..." または #include <...> を捕まえる
# 1つ目のキャプチャが quoted ("..."), 2つ目のキャプチャが angle (<...>)
inc_re = re.compile(
    r'^\s*#\s*include\s*(?:'
    r'"([^"]+)"'
    r'|'
    r'<([^>]+)>'
    r')'
)
# #pragma once を無視するためのパターン
pragma_once_re = re.compile(r'^\s*#\s*pragma\s+once\b')

# -------------------------
# ヘルパー関数群
# -------------------------

def get_include_dirs_from_compiler(compiler, compiler_opts):
    """
    コンパイラ（g++ など）に '-E -x c++ - -v' を渡して、stderr に出力される
    include search path の2セクション（quote と angle）を取り出して返す。

    引数:
      compiler: 'g++' 等の実行ファイル名またはパス
      compiler_opts: ユーザが指定する追加オプション文字列（例: '-Iinclude -DNAME'）

    返り値: (quoted_dirs, angled_dirs) の2要素タプル（いずれもリスト）

    補足:
      - shlex.split(s): シェル風に書かれた引数文字列を安全にリストに分割する。
        例: shlex.split('-I include -DNAME') -> ['-I', 'include', '-DNAME']
      - subprocess.run(cmd, capture_output=True, text=True): 外部コマンドを実行し、
        標準出力/標準エラーを回収する。text=True にすると bytes ではなく str が返る。
    """
    # compiler_opts を安全にトークン化
    opts = shlex.split(compiler_opts)
    cmd = [compiler] + opts + ['-E', '-x', 'c++', '-', '-v']
    
    # 外部コマンドを実行。ここでは stdin に空文字列を渡して EOF にする。
    # capture_output=True により proc.stdout/proc.stderr を得られる。
    proc = subprocess.run(cmd, input='', text=True, capture_output=True)

    # コンパイラが非ゼロで終了したらエラー扱いで即終了する
    if proc.returncode != 0:
        sys.stderr.write(f"ERROR: compiler returned non-zero exit {proc.returncode}\n")
        sys.stderr.write("compiler stderr:\n" + proc.stderr + "\n")
        sys.exit(proc.returncode)
    
    # stderr の中で以下の3つのマーカーとその間のパス群を抜き出す。
    #   #include "..." search starts here:
    #   (quoted paths...)
    #   #include <...> search starts here:
    #   (angled paths...)
    #   End of search list.
    # re.DOTALL: '.' が改行にもマッチする。re.MULTILINE: ^, $ を各行で効かせる。
    matched = re.search(
        r'#include "\.\.\." search starts here:\n(.*?)'
        r'^#include <\.\.\.> search starts here:\n(.*?)'
        r'^End of search list\.',
        proc.stderr, re.DOTALL | re.MULTILINE)
    
    # マッチしない場合は即エラー
    if not matched:
        sys.stderr.write("ERROR: failed to parse compiler include-search output. Expected markers not found.\n")
        sys.stderr.write("compiler stderr:\n")
        sys.stderr.write(proc.stderr + "\n")
        sys.exit(1)

    # group(1): quoted ブロックの全文（複数行）、group(2): angled ブロックの全文
    quoted = [s.strip() for s in matched.group(1).splitlines()]
    angled = [s.strip() for s in matched.group(2).splitlines()]

    # ここではコンパイラの出力そのままを返す。相対パスのままでも基本的に問題ない前提。
    return quoted, angled

def run_gpp_mm(compiler, compiler_opts, filename_or_contents, is_filename):
    """
    g++ -MM を呼んで依存を標準出力で取得する。

    引数の意味:
      - shlex.split は先ほど説明したとおり文字列を分割する関数。
      - subprocess.run(..., input=..., text=True): input に文字列を渡すとその文字列が
        外部コマンドの stdin に書き込まれる。

    戻り値: g++ -MM の stdout の文字列。エラー時はプロセスを終了する。
    """
    # shlex.split はシェル風のオプション文字列を安全に分解してリストにする関数です。
    # (例: '-DDEBUG -Ilib' -> ['-DDEBUG', '-Ilib'])
    opts = shlex.split(compiler_opts)

    # stdin からソースを渡す場合は -x c++ を付けて言語を明示
    if not is_filename:
        opts.append('-x')
        opts.append('c++')

    cmd = [compiler] + opts + ['-MM', filename_or_contents if is_filename else '-']

    # subprocess.run は指定コマンドを実行して結果を受け取る標準的手段です。
    # capture_output=True は stdout/stderr を取得するための指定（Python 3.7+）。
    # text=True にすると出力が bytes ではなく str で返ります（デコード済み）。
    try:
        # subprocess.run を使ってコマンドを実行。capture_output=True で出力を得る。
        proc = subprocess.run(cmd, input=filename_or_contents if not is_filename else None, capture_output=True, text=True)
    except FileNotFoundError as e:
        sys.stderr.write(f"Error: compiler '{compiler}' not found: {e}\n")
        sys.exit(1)

    if proc.returncode != 0:
        sys.stderr.write("g++ -MM failed (non-zero exit code).\n")
        if proc.stderr:
            sys.stderr.write("g++ stderr:\n")
            sys.stderr.write(proc.stderr)
        sys.exit(proc.returncode)
    # proc.stdout は g++ -MM の出力（文字列）
    return proc.stdout

def parse_deps_from_output(deps_output):
    """
    g++ -MM の出力から依存トークン一覧を取り出す。

    操作:
      1. バックスラッシュによる行継続（"\\\n"）を空白に置換する。
      2. 最初のコロンまで（ターゲット部分）を切り落とす。
      3. 残りを空白で split してトークン列にする。

    補足:
      - re.sub(pattern, repl, s): s の中で pattern にマッチする部分を repl に置換する。
    """
    # バックスラッシュ + 改行を空白に置き換える
    deps = re.sub(r'\\\n', ' ', deps_output)
    if ':' not in deps:
        return []
    # 「:」で始まるターゲット部分を切り落とす（最初のコロンまで）
    deps = deps.split(':', 1)[1]
    # split() で空白区切りのトークンリストへ
    tokens = deps.split()
    return tokens

def find_file(name, cur_dir, include_dirs, is_quote_form):
    """
    name を (cur_dir, include_dirs) の順で探し、見つかれば絶対パスを返す。

    include_dirs は (quoted_list, angled_list) のタプルで渡す。
    is_quote_form が True のときは quoted リストも検索する（"..." の挙動）。

    補足:
      - os.path.join(base, rel): base と rel を OS に応じて結合する。rel が絶対パスなら
        base は無視され、rel がそのまま返る（これを利用して絶対/相対の両対応をする）。
    """
    # カレント（インクルード元）ディレクトリをまず試す
    candidate = os.path.normpath(os.path.join(cur_dir, name))
    if os.path.exists(candidate):  # os.path.exists: ファイル/ディレクトリが存在するかの判定
        return os.path.abspath(candidate)  # os.path.abspath: 絶対パスに変換
    
    # include_dirs は (quoted, angled) のタプル
    quoted, angled = include_dirs
    for d in itertools.chain(quoted, angled) if is_quote_form else angled:
        candidate = os.path.normpath(os.path.join(d, name))
        if os.path.exists(candidate):
            return os.path.abspath(candidate)
    return None

def build_allowed_abs(deps_tokens):
    """
    g++ -MM で報告されたトークンが実際にファイルシステム上に存在することを厳格に確認する。
    見つからなければ即エラー終了する（fail-fast）。
    """
    allowed = set()
    for t in deps_tokens:
        if not os.path.exists(t):
            sys.stderr.write(f"Dependency reported by g++ -MM not found: {t}\n")
            sys.exit(1)
        allowed.add(os.path.abspath(t))
    return allowed

def process_file(filename_or_contents, is_filename, include_dirs, allowed_abs, inlined, result):
    """
    エントリ/ヘッダを読み、#include を見つけたら条件に応じて展開（再帰）する。

    引数:
      - filename_or_contents: ファイル名またはソース文字列
      - is_filename: True なら filename_or_contents はパス、False ならその中身（文字列）
      - include_dirs: get_include_dirs_from_compiler の返り値
      - allowed_abs: build_allowed_abs で得た展開許可された絶対パス集合
      - inlined: 既に展開済みファイルの集合（無限再帰防止）
      - result: 出力文字列を append していくリスト

    補足:
      - io.StringIO(text): 文字列をファイルのように扱うためのクラス。open() の代わりに使える。
    """
    def add_line(line):
        # 直前の要素が改行で終わっていないときは安全のために改行を追加してから行を追加
        if not line:
            return
        if result and not result[-1].endswith('\n'):
            result.append('\n')
        result.append(line)
    
    if is_filename:
        filename = os.path.abspath(filename_or_contents)
        if filename in inlined:
            return
        inlined.add(filename)
        cur_dir = os.path.dirname(filename)
    else:
        cur_dir = os.getcwd()
    try:
        # ファイルパスなら open() で読み、それ以外（stdin からのテキスト）は StringIO を使う
        with open(filename_or_contents, 'r', encoding='utf-8') if is_filename else io.StringIO(filename_or_contents) as f:
            for line in f:
                # #pragma once は完全に省く（出力に残さない）
                if pragma_once_re.match(line):
                    continue
                m = inc_re.match(line)
                if not m:
                    add_line(line)
                    continue
                # m.group(1) があれば quoted form（"..."）
                is_quote_form = m.group(1) is not None
                name = m.group(1) if is_quote_form else m.group(2)
                resolved = find_file(name, cur_dir, include_dirs, is_quote_form)
                if resolved and resolved in allowed_abs:
                    # 依存一覧に載っているユーザヘッダなので展開
                    process_file(resolved, True, include_dirs, allowed_abs, inlined, result)
                else:
                    # system ヘッダや deps にないものはそのまま残す
                    add_line(line)
    except Exception as e:
        sys.stderr.write(f"ERROR: could not open {filename if is_filename else '-'}: {e}\n")
        sys.exit(1)

# -------------------------
# main
# -------------------------

def main():
    p = argparse.ArgumentParser(description="Inline project headers listed by `g++ -MM` and print result to stdout.")
    p.add_argument('file', nargs='?', default='-', help='file file (omit or "-" to read stdin)')
    p.add_argument('--compiler', default='g++', help='C++ compiler to call (default: g++)')
    p.add_argument('--compiler-opts', default='', help='extra options passed to compiler when calling -MM (e.g. \"-std=c++17 -DNAME\")')
    args = p.parse_args()

    is_filename = args.file != '-'

    # 1) g++ -MM を呼んで依存出力を文字列として得る
    if is_filename:
        deps_output = run_gpp_mm(args.compiler, args.compiler_opts, args.file, is_filename)
    else:
        contents = sys.stdin.read()  # sys.stdin.read(): stdin から EOF まで全部読み込む
        deps_output = run_gpp_mm(args.compiler, args.compiler_opts, contents, is_filename)

    # 2) deps 出力をパースして依存トークンを得る
    deps_tokens = parse_deps_from_output(deps_output)

    # 3) deps トークン -> 実在するファイルの絶対パス集合に変換
    allowed_abs = build_allowed_abs(deps_tokens)

    # include 検索パス一覧をコンパイラから取得
    include_dirs = get_include_dirs_from_compiler(args.compiler, args.compiler_opts)

    # 4) エントリーファイルを再帰的に処理して結果を stdout に出力
    inlined = set()
    result = []
    process_file(args.file if is_filename else contents, is_filename, include_dirs, allowed_abs, inlined, result)

    result = ''.join(result)
    sys.stdout.write(result)


if __name__ == '__main__':
    main()
