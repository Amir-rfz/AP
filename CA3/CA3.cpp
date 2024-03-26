#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct User{
    string name;
    double cost;
};
struct Borrower{
    string name;
    double borrowedAmount;
};
struct Payer{
    string name;
    double paidAmount;
};
struct Expense{
    string type;
    vector<Borrower> borrowers;
    vector<Payer> payers;
};

vector<User> readUser(string filePath){
    ifstream usersFile;
    vector<User> result;
    usersFile.open(filePath);
    if(usersFile.fail())
        cerr<<"unable to open the file"<<endl;
    while(usersFile.peek()!=EOF){
        string userName;
        User user;
        user.cost=0;
        getline(usersFile , userName);
        user.name=userName;
        result.push_back(user);
    }
    usersFile.close();
    return result;
}

vector<string> splitString(string allPayer , char separator){
    int i = 0;
	vector<string> result;
    string s; 
    while (allPayer[i] != '\0') {
        if (allPayer[i] != separator) {
            s += allPayer[i]; 
        } else {
            result.push_back(s);
            s.clear();
        }
        i++;
    }
    result.push_back(s); 
	return result;
}

string getName(string payer){
    vector<string> splitPayer=splitString(payer , ':');
    string name= splitPayer[0];
    return name;
}

double getPaidAmount(string payer){
    vector<string> splitPayer=splitString(payer , ':');
    if(splitPayer.size()==1)
        return 0;
    double Paid = stold(splitPayer[1]);
    return Paid;
}

vector<Payer> getPayers(string allPayer){
    vector<Payer> result;
    Payer payer;
    vector<string> eachPayer=splitString(allPayer,';');
    for(string _payer : eachPayer){
        payer.name=getName(_payer);
        payer.paidAmount=getPaidAmount(_payer);
        result.push_back(payer);
    }
    return result;
}

vector<Borrower> getBorrowers(string allBorrowers){
    vector<Borrower> result;
    Borrower borrower;
    vector<string> eachBorrower = splitString(allBorrowers , ';');
    for(string _borrower : eachBorrower){
        borrower.name=getName(_borrower);
        borrower.borrowedAmount=getPaidAmount(_borrower);
        result.push_back(borrower);
    }
    return result;    
}

vector<Expense> readExpenses(string filePath){
    ifstream expensesFile;
    vector<Expense> result;
    expensesFile.open(filePath);
    if(expensesFile.fail())
        cerr<<"unable to open the file"<<endl;
    while(expensesFile.peek()!=EOF){
        Expense expense;

        string type;
        getline(expensesFile , type ,',');
        expense.type=type;

        string allPayers;
        getline(expensesFile , allPayers ,',');
        vector<Payer> payer = getPayers(allPayers);
        expense.payers = payer;

        string allBorrowers;
        getline(expensesFile , allBorrowers );
        vector<Borrower> borrower = getBorrowers(allBorrowers);
        expense.borrowers = borrower;  

        result.push_back(expense);
    }
    return result;
}

double findFloor(double portion){
    portion*=100;
    portion=int(portion);
    portion=double(portion);
    return portion/100;
}

double findLastPortion(double allMoney , double portion ,int numberOfUser){
    return allMoney-(portion*(numberOfUser-1));
}

void addCost(string name , double cost , vector<User>& users){
    for(User& user : users){
        if(user.name == name)
            user.cost += cost;
    }
}

void equallyCase(vector<User>& users , Expense& expense){
    if(expense.borrowers[0].name == ""){
        double portion = expense.payers[0].paidAmount / users.size();
        addCost(expense.payers[0].name , expense.payers[0].paidAmount , users);
        for(User& user : users){
            if(user.name != users[users.size()-1].name)
                user.cost -= findFloor(portion);
            else
                user.cost -= findLastPortion(expense.payers[0].paidAmount , findFloor(portion) , users.size());
        }
    }
    else{
        double _portion = expense.payers[0].paidAmount / expense.borrowers.size();
        addCost(expense.payers[0].name , expense.payers[0].paidAmount , users);
        for(Borrower borrower : expense.borrowers){
            if(borrower.name != expense.borrowers[expense.borrowers.size()-1].name)
                addCost(borrower.name , -findFloor(_portion) , users);
            else
                addCost(borrower.name , -findLastPortion(expense.payers[0].paidAmount , findFloor(_portion) , expense.borrowers.size()) , users);
        }
    }

}

void unequallyCase(vector<User>& users , Expense& expense){
    addCost(expense.payers[0].name , expense.payers[0].paidAmount , users);
    for(Borrower borrower : expense.borrowers)
        addCost(borrower.name , -borrower.borrowedAmount , users);
}

void equalApportionment(double paidAmount , vector<User>& users){
    double portion = paidAmount / users.size();
    for(User user : users)
        addCost(user.name , portion , users);
}
void  adjustmentCase(vector<User>& users , Expense& expense){
    double paidAmount=0;
    for(Payer payer : expense.payers){
        paidAmount -= payer.paidAmount;
        addCost(payer.name , payer.paidAmount , users);
    }
    for(Borrower borrower : expense.borrowers){
        paidAmount += borrower.borrowedAmount;
        addCost(borrower.name , -borrower.borrowedAmount , users);        
    }
    equalApportionment(paidAmount , users);
}

void calculateExpenses(vector<User>& users ,vector<Expense>& expenses){
    for(Expense expense : expenses){
        if(expense.type=="equally")
            equallyCase(users , expense);
        else if(expense.type=="unequally")
            unequallyCase(users , expense);
        else
            adjustmentCase(users , expense);
    }
}

void swapUser(int i , int j , vector<User>& users){
    User tempUser;
    tempUser.name = users[i].name;
    tempUser.cost = users[i].cost;

    users[i].name = users[j].name;
    users[i].cost = users[j].cost;
    
    users[j].name = tempUser.name;
    users[j].cost = tempUser.cost;
}

void sorting(vector<User>& users){
    for(int i=0 ; i<users.size() ; i++){
        for(int j=i+1 ; j<users.size() ; j++){
            if(users[i].cost<users[j].cost)
                swapUser(i , j , users);
            else if(users[i].cost==users[j].cost && users[i].name < users[j].name)
                swapUser(i , j , users);
        }
    }
}

void Optimization(vector<User>& users , int beginCreditor , int beginDebtor , vector<User>& result){
    User payment;
    if(beginCreditor == beginDebtor)
        return;
    
    if(users[beginCreditor].cost + users[beginDebtor].cost == 0){
        payment.name = users[beginDebtor].name + " -> " + users[beginCreditor].name + ": ";
        payment.cost = users[beginCreditor].cost;
        result.push_back(payment);
        users[beginCreditor].cost = 0; 
        users[beginDebtor].cost = 0;
        Optimization(users , beginCreditor+1 , beginDebtor-1 , result);
    }
    else if(users[beginCreditor].cost + users[beginDebtor].cost < 0){
        payment.name = users[beginDebtor].name + " -> " + users[beginCreditor].name + ": ";
        payment.cost = users[beginCreditor].cost;
        result.push_back(payment);
        users[beginDebtor].cost = users[beginCreditor].cost + users[beginDebtor].cost;
        users[beginCreditor].cost = 0;
        Optimization(users , beginCreditor+1 , beginDebtor , result);
    }
    else{
        payment.name = users[beginDebtor].name + " -> " + users[beginCreditor].name + ": ";
        payment.cost = abs(users[beginDebtor].cost);
        result.push_back(payment);
        users[beginCreditor].cost = users[beginCreditor].cost + users[beginDebtor].cost;
        users[beginDebtor].cost = 0;
        Optimization(users , beginCreditor , beginDebtor-1 , result);        
    }
}

int main(int argc, char* argv[]){
    vector<User> users=readUser(argv[1]);
    vector<Expense> expenses=readExpenses(argv[2]);
    vector<User> payments;
    calculateExpenses(users , expenses);
    sorting(users);
    Optimization(users , 0 , users.size()-1 , payments);
    sorting(payments);
    for(auto payment : payments)
        cout << payment.name << payment.cost << endl;
    return 0;
}