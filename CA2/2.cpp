#include<iostream>
#include<string>
#include<vector>
using namespace std;
struct item{
    vector<int> childeren;
    string name;
};

/*this function gets the index of an item and return the index of his father*/
int find_father(vector<item> items,int i){
    for(int j=0;j<items.size();j++){
        for(auto child:items[j].childeren){
            if(child==i)
                return j;
        }
    }
    return -1;
}
/*this function check that if a item is the last item in the children of his father or not
that becuase when its last one we shoude print"   " and if not print "|  "*/
bool is_last(vector<item> items,int item){
    if(item==0)
        return true;
    int father_index=find_father(items,item);
    int last_index=items[father_index].childeren.size()-1;
    if(items[father_index].childeren[last_index]==item)
        return true;
    return false;
}
/*this function gets the data from the input 
at first we save the index of father of each item in a vector and then in another function we find 
the children of each item */
void get_data(vector<item>& items,vector<int>& reference,int num_of_items){
    for(int i=1;i<=num_of_items;i++){
        item someitem;
        int ref;
        cin>>someitem.name>>ref;
        items.push_back(someitem);
        reference.push_back(ref);
    }
}
/*in this function we find the children of each item by using the refrence vector*/
void get_childeren(vector<item>& items,vector<int> reference){
    for(int i=1;i<reference.size();i++)
        items[reference[i]].childeren.push_back(i);
}

/* "   " is 1 and "|  "is zero
in this function we print eche child recursivly and for the line i used a vector that contain 0 and 1 that when
its see 1 then print "   " and when see 0 print "|  "*/  
void print(vector<item> items,int i,vector<int> depth){
    if(i!=0){
        if(depth.size()<=4){
            for(auto type : depth){
                if(type==1)
                    cout<<"   ";
                else
                    cout<<"|  ";
            }
            cout<<"|__"<<items[i].name << endl;

            if(is_last(items , i))
                depth.push_back(1);
            else
                depth.push_back(0);
        }
    }
    for(int child: items[i].childeren)
        print(items,child,depth);
}
int main(){
    item root;
    vector<item> items;items.push_back(root);
    vector<int>reference;reference.push_back(0);
    vector<int> depth;
    int num_of_items,max_depth;
    cin>>num_of_items>>max_depth;
    get_data(items,reference,num_of_items);
    get_childeren(items,reference);
    print(items,0,depth);
}