#include<iostream>
#include<string>
#include<vector>
using namespace std;

//making a structur for each lesson we want to get this term
struct lessons{
    string course_name;
    int units;
    string prerequisites;
};

//this function gets the lesson that Offered in this term from input
vector<lessons> get_this_term_lesson(int num){
    vector<lessons> result;
    for(int i=0;i<num;i++){
        lessons lesson;
        cin>>lesson.course_name>>lesson.units>>lesson.prerequisites;
        result.push_back(lesson);
    }
    return result;
}

//making a structur for each lesson we had in past
struct past_lessons{
    string course_name;
    int units;
    double grade;
};

//this function gets the lessons that we had in past from input
vector<past_lessons> get_past_lesson(int num){
    vector<past_lessons> result;
    for(int i=0;i<num;i++){
        past_lessons lesson;
        cin>>lesson.course_name>>lesson.units>>lesson.grade;
        result.push_back(lesson);
    }
    return result;
}

//this function gets the lesson that we want to get in upcoming term
vector<string>get_wanted_lessons(int num){
    vector<string> result;
    for (int i=0;i<num;i++){
        string lesson;
        cin>> lesson;
        result.push_back(lesson);
    }
    return result;
}

//this function check that the course that we want is offered this term or not
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

//this function count our units that we want to get this term
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

//this function check that is our units is more than 12 or not
bool check_min_units(vector<string> wanted_lessons,vector<lessons> this_term_lesson){
    int units=number_of_unit(wanted_lessons,this_term_lesson);
    if(units>=12)
        return true;
    else
        return false;
}

//this function calculate our GPA 
double GPA(vector<past_lessons> past_lesson){
    double sum=0;
    for(int i=0;i<past_lesson.size();i++){
        sum+=past_lesson[i].grade;
    }
    double GPA=sum/past_lesson.size();
    return GPA;
}

//this function check that we dont get more than maximum utit we can get with our GPA
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

//this function check that is a course taken before or not
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

//this function splite the Prerequisites from ',' to a vector of strings
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

//this function return a vector of all course we passed 
vector<string>get_pass_lesssons(vector<past_lessons> past_lessons){
    vector<string>pass_lessons;
    for(auto past_lesson:past_lessons){
        if(past_lesson.grade>=10)
            pass_lessons.push_back(past_lesson.course_name);
    }
    return pass_lessons;
}

//this function return all Prerequisites that a course had in a string in form: <name1,name2,...>
string get_Prerequisites(string lesson_name,vector<lessons> this_term_lesson){
    for(auto lesson : this_term_lesson){
        if(lesson_name==lesson.course_name)
            return lesson.prerequisites;
    }
    return " ";
}

//this function check that did we pass the Prerequisite or not
bool Prerequisites_isnt_Met(string Prerequisite ,vector<string>pass_lessons){
    for(auto pass_lesson : pass_lessons){
        if(pass_lesson==Prerequisite)
            return false;
    }
    return true;
}

//this function check that did we pass all Prerequisites of lessons we want to get this term
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