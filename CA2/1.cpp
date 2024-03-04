#include<iostream>
#include<cmath>
#include<cstdlib>
using namespace std;

//this is a functio that give us the number of digit a number
int num_of_digit(int number){
    int digits=1;
    number=abs(number);
    while(number>=10){
        digits++;
        number/=10;
    }
    return digits;
}

//this function give us a number with all 1 digit
int make_1_number(int num_of_digit){
    if(num_of_digit<=1)
        return 1;
    else
        return (pow(10,num_of_digit-1) + make_1_number(num_of_digit-1));
}

//this function give us the abs difference of two number
int dif(int x,int y){
    return abs(abs(x)-abs(y));
}

//this function find the closest all 1 digits number to a number(closest mean the value of number(abs))
int find_closest(int number){
    int n=num_of_digit(number);
    int num1=make_1_number(n-1);
    int num2=make_1_number(n);
    int num3=make_1_number(n+1);
    if(dif(number,num1)<=dif(number,num2) && dif(number,num1)<=dif(number,num3))
        return num1;
    else if(dif(number,num2)<=dif(number,num1) && dif(number,num2)<=dif(number,num3))
        return num2;
    else
        return num3;
}

//this function return the number of digit that we need to make the number(result of problem)
int min_digit_1_to_made(int input){
    if(input==0)
        return 0;
    else if(input<0)
        return  num_of_digit(find_closest(input)) + min_digit_1_to_made(input+find_closest(input));
    else
        return num_of_digit(find_closest(input)) + min_digit_1_to_made(input-find_closest(input));

}

int main(){
    int input;
    cin>>input;
    cout<<min_digit_1_to_made(input);
    return 0;
}