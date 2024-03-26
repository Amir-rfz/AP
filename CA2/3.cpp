#include<iostream>
#include<vector>
 
using namespace std;

struct train{
    vector<int> station;
};

struct Point{
    int train;
    int station;
};

struct RouteCost{
    Point current;
    Point next;
    int cost;
};

train get_train(){
    train result;
    int number_of_station;
    cin>>number_of_station;
    for(int i=0;i<number_of_station;i++){
        int status;
        cin>>status;
        result.station.push_back(status);
    }
    return result;
}
void get_data(int number_of_line , vector<train>& trains){
    for(int i=0;i<number_of_line;i++){
        train _train;
        _train=get_train();
        trains.push_back(_train);
    }
}
void get_point(Point& point){
    int cur_train , cur_station;
    cin>>cur_train>>cur_station;
    point.train=cur_train-1;
    point.station=cur_station-1;
}

RouteCost change_line(vector<train> trains,Point start){
    RouteCost result;
    result.current=start;
    result.cost=2;
    result.next.train=trains[start.train].station[start.station]-1;
    for(int i=0;i<trains[result.next.train].station.size();i++){
        if(trains[result.next.train].station[i]==start.train+1)
            result.next.station=i;
    }
    return result;
}


bool is_intersection(vector<train> trains,Point start){
    if(trains[start.train].station[start.station]!=0)
        return true;
    return false;
}

vector<RouteCost> posible_single_moves(vector<train> trains,Point start){
    vector<RouteCost> result;
    RouteCost move1,move2;
    move1.current=start;
    move1.next.station= start.station+1;
    move1.next.train=start.train;
    move1.cost=1;
    result.push_back(move1);

    move2.current=start;
    move2.next.station= start.station+1;
    move2.next.train=start.train;
    move2.cost=1;
    result.push_back(move2);

    if(is_intersection(trains,start)){
        RouteCost move3;
        move3=change_line(trains,start);
        result.push_back(move3);
    }
    return result;
}
int find_min_cost(vector<int> costs){
    if(costs.size()==0)
        return -1;
    int min_cost=costs[0];
    for(int i=1;i<costs.size();i++){
        if(costs[i]<=min_cost)
            min_cost=costs[i];
    }
    return min_cost;
}
bool is_equal(Point first_point , Point second_point){
    if(first_point.station==second_point.station && first_point.train==second_point.train)
        return true;
    return false;
}
bool is_checked(vector<Point> pre_points,Point start){
    for(auto point : pre_points){
        if(is_equal(point,start))
            return true;
    }
    return false;
}
int solve(vector<train> trains,Point start,Point goal,vector<Point>& pre_points){
    if(start.station==goal.station && start.train==goal.train)
        return 0; 
    vector<int> costs;
    for(auto move:posible_single_moves(trains ,start)){
        Point new_start;
        new_start.station=move.next.station;
        new_start.train=move.next.train;
        if(new_start.station<0 || new_start.train>=trains.size() || new_start.train<0 || new_start.station>=trains[new_start.train].station.size())
            continue;
        if(is_checked(pre_points,new_start))
            continue;
        pre_points.push_back(new_start);
        int child_cost=solve(trains,new_start,goal,pre_points);
        if(child_cost!=-1){
            costs.push_back(move.cost+child_cost);
        }
        pre_points.pop_back();
    }

    return find_min_cost(costs);
}

int main(){
    vector<train> trains;
    Point start,goal;
    vector<Point> pre_points;
    int time_cost=0;
    int number_of_line;
    cin>>number_of_line;
    get_data(number_of_line,trains);
    get_point(start);
    get_point(goal);
    pre_points.push_back(start);
    cout<<solve(trains,start,goal,pre_points);
    return 0;
}