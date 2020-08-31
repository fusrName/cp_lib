std::vector<int> sieve(int n) {
    std::vector<int> min_factor(n+1);
    for(int i=2;i<=n;i++)min_factor[i]=i;
    for(int i=4;i<=n;i+=2)min_factor[i]=2;
    for(int i=9;i<=n;i+=3)min_factor[i]=3;
    bool f=true;
    for(int p=5;p*p<=n;p+=f?2:4,f=!f)
        if(min_factor[p]==p) {
            for(int i=p*p;i<=n;i+=p)
                if(min_factor[i]==i)
                    min_factor[i]=p;
        }
    return min_factor;
};
