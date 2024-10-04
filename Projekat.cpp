#include "bits/stdc++.h"
#include <time.h>
#include <random>
#define inf 1000000
using namespace std;
int seed;
random_device rd;  
mt19937 gen(rd());
double _time(){
    return 1.0*clock()/CLOCKS_PER_SEC;
}
void error(){
    fprintf(stderr,"error occurred\n");
    exit(EXIT_FAILURE);
}
void move_forward(vector<int>& path,int v1,int v2,int n){
    for(int i=v2+n;(i%n)!=((v1+n-1)%n);i--){
        path[(i+1)%n]=path[i%n];
    }
}
void roulette(int n,vector<int>& route){
    int a[n];
    for(int i=0;i<n;i++){
        a[i]=i;
    }
    int pos;
    while(n>0){
        pos=gen()%n;
        route.push_back(a[pos]);
        a[pos]=a[n-1];
        n--;
    }
}
void intern_create_path(double** graph,vector<int> cities,vector<int> &path,int n,double& total_distance){
        int current=0;
        int next;
        total_distance=0;
        bool visited[n];
        for(int i=0;i<n;i++){
            visited[i]=false;
        }
        path.push_back(cities[current]);
        visited[current]=true;
        for(int i=0;i<n-1;i++){
            next=current;
            for(int j=0;j<n;j++){
                if(!visited[j]&&graph[cities[current]][cities[next]]>graph[cities[current]][cities[j]]){
                    next=j;
                }
            }
            path.push_back(cities[next]);
            total_distance+=graph[cities[current]][cities[next]];
            visited[next]=true;
            current=next;
        }
        total_distance+=graph[cities[current]][cities[0]];
}
void put_city(double** graph,vector<int> &path,int path_lenght[],double& total_distance,int color,int place,int city){
    total_distance+=(graph[path[place]][city]+graph[city][path[(place+1)%path_lenght[color]]]-graph[path[place]][path[(place+1)%path_lenght[color]]]);
    path.push_back(city);
    path_lenght[color]++;
    if(place!=path_lenght[color]-2){
        move_forward(path,place+1,path_lenght[color]-2,path_lenght[color]);
        path[place+1]=city;
    }
}
void add_city(double** graph,vector<int> paths[],int m,double& total_distance,int city,int path_lenght[]){
    double increase=inf;
    int color=0;
    int place=0;
    int first,second,n;
    for(int i=0;i<m;i++){
        n=path_lenght[i];
        for(int j=0;j<n;j++){
            first=paths[i][j];
            second=paths[i][(j+1)%n];
            if(increase>(graph[first][city]+graph[city][second]-graph[first][second])){
                increase=(graph[first][city]+graph[city][second]-graph[first][second]);
                color=i;
                place=j;
            }
        }
    }
    put_city(graph,paths[color],path_lenght,total_distance,color,place,city);
}
void create_path(double** graph,vector<int> paths[],vector<int> cities[],int n,int k_m,int m,int path_lenght[],double& total_distance){
    double distance[m];
    for(int i=0;i<m;i++){
        intern_create_path(graph,cities[i],paths[i],k_m+1,distance[i]);
        path_lenght[i]+=k_m+1;
        total_distance+=distance[i];
    }
    for(int i=0;i<n-1-m*k_m;i++){
        add_city(graph,paths,m,total_distance,cities[m][i],path_lenght);
    }
}
void remove_city(double** graph,vector<int> &path,int position,int n,double& total_distance){
    total_distance-=(graph[path[(position+n-1)%n]][path[position]]+graph[path[position]][path[(position+1)%n]]-graph[path[(position+n-1)%n]][path[(position+1)%n]]);
    for(int i=position+1;i<n;i++){
        path[i-1]=path[i];
    }
    path.pop_back();
}
void shake(double** graph,vector<int> paths[],int m,int path_lenght[],int k,double& total_distance,bool free_city[]){
    for(int i=0;i<k;i++){
        label1:
        int color=gen()%m;
        int position=gen()%path_lenght[color];
        int city=paths[color][position];
        if(city==0){
            goto label1;
        }
        if(free_city[city]){
            remove_city(graph,paths[color],position,path_lenght[color],total_distance);
            path_lenght[color]--;
            color=gen()%m;
            position=gen()%path_lenght[color];
            put_city(graph,paths[color],path_lenght,total_distance,color,position,city);
        }
        else{
            remove_city(graph,paths[color],position,path_lenght[color],total_distance);
            path_lenght[color]--;
            position=gen()%path_lenght[color];
            put_city(graph,paths[color],path_lenght,total_distance,color,position,city);
        }
    }
}
void reverse(vector<int> &path,int start,int end,int n){
    while(end>start){
        int temp=path[start%n];
        path[start%n]=path[end%n];
        path[end%n]=temp;
        start++;
        end--;
    }
}
int is_shorther(double** graph,int v1,int v2,int v3,int v4,double& total_distance){
    if(v1==v3||v1==v4||v2==v3||v2==v4){
        return 0;
    }
    if((graph[v1][v3]+graph[v2][v4])<(graph[v1][v2]+graph[v3][v4])){
        total_distance-=((graph[v1][v2]+graph[v3][v4])-(graph[v1][v3]+graph[v2][v4]));
        return 1;
    }
    return 0;
}
int is_shorther_insert(double** graph,vector<int>&path,int v1,int v2,int n,double& total_distance){
    int a=path[v1%n];
    int b=path[(v1+1)%n];
    int c=path[(v1+2)%n];
    int d=path[v2%n];
    int e=path[(v2+1)%n];
    if(b==d||b==e){
        return 0;
    }
    if((graph[a][c]+graph[b][d]+graph[b][e])<(graph[a][b]+graph[b][c]+graph[d][e])){
        total_distance-=((graph[a][b]+graph[b][c]+graph[d][e])-(graph[a][c]+graph[b][d]+graph[b][e]));
        return 1;
    }
    else{
        return 0;
    }
}
int one_opt(double** graph,vector<int>& path,int n,double& total_distance){
    if(n<4){
        return 0;
    }
    vector<int> route;
    roulette(n,route);
    int p;
    for(int i=0;i<n;i++){
        p=route[i];
        if(is_shorther(graph,path[p],path[(p+1)%n],path[(p+2)%n],path[(p+3)%n],total_distance)){
            reverse(path,p+1,p+2,n);
            return 1;
        }
    }
    return 0;
}
int two_opt(double** graph,vector<int>& path,int n,double& total_distance){
    if(n<4){
        return 0;
    }
    vector<int> route;
    roulette(n,route);
    int p,l;
    for(int i=0;i<n;i++){
        p=route[i];
        for(int j=i+1;j%n!=i;j++){
            l=route[j%n];
            if(is_shorther(graph,path[p],path[(p+1)%n],path[(l%n)],path[(l+1)%n],total_distance)){
                if(l<p){
                    l+=n;
                }
                reverse(path,p+1,l,n);
                return 1;
            }
        }
    }
    return 0;
}
int insert_opt(double** graph,vector<int>& path,int n,double& total_distance){
    if(n<4){
        return 0;
    }
    vector<int> route;
    roulette(n,route);
    int p,l;
    for(int i=0;i<n;i++){
        p=route[i];
        for(int j=i+1;j%n!=i;j++){
            l=route[j%n];
            if(is_shorther_insert(graph,path,p,l,n,total_distance)){
                int pom=path[(p+1)%n];
                move_forward(path,l+1,p,n);
                path[(l+1)%n]=pom;
                return 1;
            }
        }
    }
    return 0;
}
int better_place(double** graph,vector<int> paths[],int m,int path_lenght[],int color,int position){
    int k=path_lenght[color];
    int city=paths[color][position];
    double change=graph[paths[color][(position+k-1)%k]][paths[color][(position+1)%k]]-graph[paths[color][(position+k-1)%k]][city]-graph[city][paths[color][(position+1)%k]];
    for(int i=0;i<m;i++){
        if(i==color){
            continue;
        }
        k=path_lenght[i];
        for(int j=0;j<path_lenght[i];j++){
            if(change+graph[paths[i][j]][city]+graph[city][paths[i][(j+1)%k]]-graph[paths[i][j]][paths[i][(j+1)%k]]<0){
                return 1;
            }
        }
    }
    return 0;
}
int inter_search(double** graph,vector<int> paths[],int m,int path_lenght[],bool free_city[],double& total_distance){
    double change;
    int k;
    int city;
    for(int i=0;i<m;i++){
        for(int j=0;j<path_lenght[i];j++){
            if(paths[i][j]==0){
                continue;
            }
            if(free_city[paths[i][j]]){
                if(better_place(graph,paths,m,path_lenght,i,j)){
                    city=paths[i][j];
                    if(city==0){
                        printf("greska\n");
                    }
                    remove_city(graph,paths[i],j,path_lenght[i],total_distance);
                    path_lenght[i]--;
                    add_city(graph,paths,m,total_distance,city,path_lenght);
                    return 1;
                }
            }
        }
    }
    return 0;
}
void local_search(double** graph,vector<int> paths[],int m,int path_lenght[],bool free_city[],double& total_distance){
        label:
        for(int i=0;i<m;i++){
            if(one_opt(graph,paths[i],path_lenght[i],total_distance)){
                goto label;
            }
        }
        for(int i=0;i<m;i++){
            if(insert_opt(graph,paths[i],path_lenght[i],total_distance)){
                goto label;
            }
        }
        for(int i=0;i<m;i++){
            if(two_opt(graph,paths[i],path_lenght[i],total_distance)){
                goto label;
            }
        }
        if(inter_search(graph,paths,m,path_lenght,free_city,total_distance)){
            goto label;
        }
}
void read(char* file_name,double*& x,double*& y,int& n,int& m,int& k_m){
    FILE* inputf=fopen(file_name,"r"); 
    if(inputf==NULL){
        error();
    }
    char aa[1000];
    int s;
    fscanf(inputf,"%s %d %d %d",aa,&n,&m,&k_m);
    n++;
    x=new double[n];
    y=new double[n];
    for(int i=0;i<n;i++){
        fscanf(inputf,"%d %lf %lf",&s,&x[i],&y[i]);
    }
    fclose(inputf);
}
int main(int argc,char* argv[]){
    if(argc!=4){
        error();
    }
    double time_active=atof(argv[2]);
    remove("izlaz.txt");
    FILE* izlaz=fopen("izlaz.txt","a");
    for(int seed=4;seed<=4;seed++){
        fprintf(izlaz,"seed: %d    ",seed);
        gen.seed(seed);
        int n,m,k_m;
        double* x;
        double* y;
        read(argv[1],x,y,n,m,k_m);
        vector<int> cities[m+1];
        int index=1;
        for(int i=0;i<m;i++){
            cities[i].push_back(0);
            for(int j=0;j<k_m;j++){
                cities[i].push_back(index);
                index++;
            }
        }
        while(index<n){
            cities[m].push_back(index);
            index++;
        }
        bool free_city[n];
        index=1;
        for(int i=0;i<n;i++){
            free_city[i]=true;
        }
        for(int i=0;i<m;i++){
            for(int j=0;j<k_m;j++){
                free_city[index]=false;
                index++;
            }
        }
        vector<int> paths[m];
        double total_distance=0;
        double** graph=new double*[n];
        for(int i=0;i<n;i++){
            graph[i]=new double[n];
        }
        if (n<=201 || (n>901 && n<5001)){
            for (int i=0;i<n;i++)
                for (int j=0;j<n;j++)
                        graph[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
        }
        if (n>=5001){
            for (int i=0;i<n;i++)
                for (int j=0;j<n;j++)
                    graph[i][j]=ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
        }
        if (n>201 && n<701){
            double q1,q2,q3;
            double RRR = 6378.388;
            double latitude[n];
            double longitude[n];
            double min;
            double deg;
            double PI=3.141592;
            for (int i=0;i<n;i++){
                deg=int(x[i]);
                min=x[i]-deg;
                latitude[i]=PI * (deg + 5.0 * min / 3.0 ) / 180.0;
                deg=int(y[i]);
                min=y[i]-deg;
                longitude[i]=PI * (deg + 5.0 * min / 3.0 ) / 180.0;
            }
            for (int i=0;i<n;i++)
                for (int j=0;j<n;j++){
                    q1 = cos( longitude[i] - longitude[j] );
                    q2 = cos( latitude[i] - latitude[j] );
                    q3 = cos( latitude[i] + latitude[j] );
                    graph[i][j] = (int) ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
                }
        }
        for(int i=0;i<n;i++){
            graph[i][i]=inf;
        }
        int path_lenght[m];
        for(int i=0;i<m;i++){
            path_lenght[i]=0;
        }
        
        create_path(graph,paths,cities,n,k_m,m,path_lenght,total_distance);
        double start_time=_time();    
        local_search(graph,paths,m,path_lenght,free_city,total_distance);
        int min_k=n/100+1;
        int k=min_k;
        int max_k=n/5+1;
        vector<int> new_paths[m];
        int new_path_lenght[m];
        double new_total_distance;
        while(_time()-start_time<time_active){
            for(int i=0;i<m;i++){
                new_paths[i].clear();
                new_path_lenght[i]=path_lenght[i];
                for(int j=0;j<path_lenght[i];j++){
                    new_paths[i].push_back(paths[i][j]);
                }
            }
            new_total_distance=total_distance;
            shake(graph,new_paths,m,new_path_lenght,k,new_total_distance,free_city);
            local_search(graph,new_paths,m,new_path_lenght,free_city,new_total_distance);
            if(_time()-start_time>time_active){
                break;
            }
            if(new_total_distance<total_distance){
                k=min_k;
                total_distance=new_total_distance;
                for(int i=0;i<m;i++){
                    path_lenght[i]=new_path_lenght[i];
                    paths[i].clear();
                    for(int j=0;j<path_lenght[i];j++){
                        paths[i].push_back(new_paths[i][j]);
                    }
                }
            }
            else{
                k+=1;
            }
            if(k>max_k){
                k=min_k;
            }
        }
        fprintf(izlaz,"best total distance is: %lf\n",total_distance);
        /*int depot;
        for(int i=0;i<m;i++){
            printf("put %d: ",i+1);
            for(int j=0;j<path_lenght[i];j++){
                if(paths[i][j]==0){
                    depot=j;
                    break;
                }
            }
            for(int j=0;j<path_lenght[i];j++){
                printf("%d ",paths[i][(depot+j)%path_lenght[i]]);
            }
            printf("\n");
        }*/
        free(x);
        free(y);
    }
    fclose(izlaz);
    printf("GOTOVO\n");
}