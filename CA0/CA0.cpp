#include<iostream>
#include<vector>
#include<string>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    //getting the train list
    string train_name,source,destination,time="";
    int remaining_capacity;
    vector<string> train_names,sources,destinations,times;
    vector<int>remaining_capacities;
    for(int i=0;i<n;i++){
        cin>>train_name>>source>>destination>>time>>remaining_capacity;
        train_names.push_back(train_name);
        sources.push_back(source);
        destinations.push_back(destination);
        times.push_back(time);
        remaining_capacities.push_back(remaining_capacity);
    }
    //getting the tickets
    string ticket_source,ticket_destination,first_possible_time="";
    int count;
    vector<string> ticket_sources,ticket_destinations,first_possible_times;
    vector<int>counts;
    for(int i=0;i<m;i++){
        cin>>ticket_source>>ticket_destination>>first_possible_time>>count;
        ticket_sources.push_back(ticket_source);
        ticket_destinations.push_back(ticket_destination);
        first_possible_times.push_back(first_possible_time);
        counts.push_back(count);
    }
    //show the output
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(ticket_sources[i]==sources[j] && ticket_destinations[i]==destinations[j] && counts[i]<=remaining_capacities[j]){
                if(first_possible_times[i]<=times[j])
                    cout<<train_names[j]<<" "<<times[j]<<" "<<remaining_capacities[j]<<endl;
            }
        }
        cout<<"----------"<<endl;
    }
    return 0;
}
