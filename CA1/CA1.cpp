#include<iostream>
#include<string>
#include<vector>
using namespace std;
struct lessons{
    string course_name;
    int units;
    string prerequisites;
};
vector<lessons> get_this_term_lesson(int num){
    vector<lessons> result;
    for(int i=0;i<num;i++){
        lessons lesson;
        cin>>lesson.course_name>>lesson.units>>lesson.prerequisites;
        result.push_back(lesson);
    }
    return result;
}
struct past_lessons{
    string course_name;
    int units;
    double grade;
};
vector<past_lessons> get_past_lesson(int num){
    vector<past_lessons> result;
    for(int i=0;i<num;i++){
        past_lessons lesson;
        cin>>lesson.course_name>>lesson.units>>lesson.grade;
        result.push_back(lesson);
    }
    return result;
}
vector<string>get_wanted_lessons(int num){
    vector<string> result;
    for (int i=0;i<num;i++){
        string lesson;
        cin>> lesson;
        result.push_back(lesson);
    }
    return result;
}
bool check_ofered(vector<string> wanted_lessons,vector<lessons> this_term_lesson){
    for(int i=0;i<wanted_lessons.size();i++){
        bool ofered=false;
        for(int j=0;j<this_term_lesson.size();j++){
            if(wanted_lessons[i]==this_term_lesson[j].course_name)
                ofered=true;
        }
        if(!ofered)
            return false;
    }
    return true;
}
int number_of_unit(vector<string> wanted_lessons,vector<lessons> this_term_lesson){
    int units=0;
    for(int i=0;i<wanted_lessons.size();i++){
        for(int j=0;j<this_term_lesson.size();j++){
            if(wanted_lessons[i]==this_term_lesson[j].course_name)
                units+=this_term_lesson[j].units;
        }
    }
    return units;
}
bool check_min_units(vector<string> wanted_lessons,vector<lessons> this_term_lesson){
    int units=number_of_unit(wanted_lessons,this_term_lesson);
    if(units>=12)
        return true;
    else
        return false;
}
double GPA(vector<past_lessons> past_lesson){
    double sum=0;
    for(int i=0;i<past_lesson.size();i++){
        sum+=past_lesson[i].grade;
    }
    double GPA=sum/past_lesson.size();
    return GPA;
}
bool check_max_units(vector<lessons> this_term_lesson,vector<past_lessons> past_lesson,vector<string> wanted_lessons){
    double gpa=GPA(past_lesson);
    int unit_number=number_of_unit(wanted_lessons,this_term_lesson);
    if(gpa>=17 && unit_number<=24)
        return true;
    if(12<=gpa<17 && unit_number<=20)
        return true;
    if(gpa<12 && unit_number<=14)
        return true;
    else    
        return false;
}
bool is_taken_course(vector<string> wanted_lessons,vector<past_lessons> past_lesson){
    bool taken;
    for(int i=0;i<wanted_lessons.size();i++){
        taken=false;
        for(int j=0;j<past_lesson.size();j++){
            if(wanted_lessons[i]==past_lesson[j].course_name){
                if(past_lesson[j].grade>=10)
                    taken=true;
            }
        }
        if(taken)
            return true;
    }
    return false;
}
vector<string> splite_Prerequisites(string Prerequisites){
    vector<string> result;
    string Prerequisite="";
    for(auto x : Prerequisites){
        if(x==','){
            result.push_back(Prerequisite);
            Prerequisite="";
            continue;
        }
        else
            Prerequisite=Prerequisite+x;
    }
    result.push_back(Prerequisite);
    return result;
}
vector<string>get_pass_lesssons(vector<past_lessons> past_lessons){
    vector<string>pass_lessons;
    for(auto past_lesson:past_lessons){
        if(past_lesson.grade>=10)
            pass_lessons.push_back(past_lesson.course_name);
    }
    return pass_lessons;
}
string get_Prerequisites(string lesson_name,vector<lessons> this_term_lesson){
    for(auto lesson : this_term_lesson){
        if(lesson_name==lesson.course_name)
            return lesson.prerequisites;
    }
    return " ";
}
bool Prerequisites_isnt_Met(string Prerequisite ,vector<string>pass_lessons){
    for(auto pass_lesson : pass_lessons){
        if(pass_lesson==Prerequisite)
            return false;
    }
    return true;
}
bool check_Prerequisites(vector<string> wanted_lessons,vector<past_lessons> past_lesson,vector<lessons> this_term_lesson){
    vector<string>pass_lessons=get_pass_lesssons(past_lesson);
    for(int i=0;i<wanted_lessons.size();i++){
        string all_Prerequisites=get_Prerequisites(wanted_lessons[i],this_term_lesson);
        vector<string>Prerequisites= splite_Prerequisites(all_Prerequisites);
        for(auto x:Prerequisites){
            if(Prerequisites_isnt_Met(x,pass_lessons))
                return false;
        }
    }
    return true;
}
int main(){
    vector<lessons> this_term_lesson;
    vector<past_lessons> past_lesson;
    vector<string> wanted_lessons;
    //getting the lessons for upcoming term
    int num_of_this_term_lesson;
    cin>>num_of_this_term_lesson;
    this_term_lesson=get_this_term_lesson(num_of_this_term_lesson);
    //getting the lessons that we had 
    int num_of_past_lesson;
    cin>>num_of_past_lesson;
    past_lesson= get_past_lesson(num_of_past_lesson);
    //get the lessons we want to get this term
    int num_of_wanted_lesson;
    cin>>num_of_wanted_lesson;
    wanted_lessons= get_wanted_lessons(num_of_wanted_lesson);
    //check if we can get this lessons or not
    if(!check_ofered(wanted_lessons,this_term_lesson))
        cout<<"Course Not Offered!";
    else if(!check_min_units(wanted_lessons,this_term_lesson))
        cout<<"Minimum Units Validation Failed!";
    else if(!check_max_units(this_term_lesson,past_lesson,wanted_lessons))
        cout<<"Maximum Units Validation Failed!";
    else if(is_taken_course(wanted_lessons,past_lesson))
        cout<<"Course Already Taken!";
    else if(!check_Prerequisites(wanted_lessons,past_lesson,this_term_lesson))
        cout<<"Prerequisites Not Met!";
    else
        cout<<"OK!";
    return 0;
}