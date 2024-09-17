#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>  //sort function
#include "CA4.h"

using namespace std;

void Car::setStatus(string _status){
    status = _status;
}

void Car::moveToNextStage(){
    currentStageIndex +=1;
    if(currentStageIndex == stages.size())
        status = "Done";

}


void Worker::setStatus(string _status){
    status = _status;
}

Car* Worker::goToNextTime(){
    appendTime +=1;
    if(appendTime == timeToFinish){
        appendTime = 0;
        status = "Idle";
        return beingWashedCar;
    }
    return NULL;
}

Worker::Worker(int _id,int _stagedId ,int _timeToFinish){
    id = _id;
    stageId = _stagedId;
    timeToFinish = _timeToFinish;
    appendTime = 0;
    status = "Idle";
}

string Worker::getStatus(){
    return status;
}

void Stage::choseFromQueue(){
    if(queue.size() == 0){
        return;
    }
    for(auto worker : Workers){
        if(worker->getStatus() == "Idle"){
            Car* firstCar = queue[0];
            assignCarToWorker(worker , firstCar);
            queue.erase(queue.begin());
            cout << to_string(time->getTime()) + " Car " + to_string(firstCar->getID()) + " Queue " + to_string(id) + " -> Stage " + to_string(id) << endl;
        }
    }
}

void Stage::assignCarToWorker(Worker* worker, Car* car){
    worker->getNewCar(car);
    worker->setStatus("Working");
    car->setStatus("In service: Stage " + to_string(id));
    carsBeingWashed += 1;
}

void Stage::addOneCarWashed(){
    income += price;
    washedCar += 1;
    carsBeingWashed -= 1;
}

vector<Car*> Stage::increaseTime(){
    vector<Car*> result;
    for(auto worker : Workers){
        if(worker->getStatus() == "Working"){
            Car* doneCar=worker->goToNextTime();
            if(doneCar != NULL){
                result.push_back(doneCar);
                addOneCarWashed();
                choseFromQueue();
            }
        }
    }
    return result;
}

void Stage::addWorker(Worker* worker){
    Workers.push_back(worker);
}

void Stage::showStatus(){
    cout << "Number of washed cars: " << washedCar << endl;
    cout << "Number of cars in queue: " << queue.size() << endl;
    cout << "Number of cars being washed: " << carsBeingWashed << endl;
    cout << "Income: " << income << endl;
}

Stage::Stage(int _id , int _price , Time* _time){
    id = _id;
    price = _price;
    time = _time;
    washedCar = 0;
    carsBeingWashed = 0;
    income = 0;
}

bool Stage::allWorkersAreBusy(){
    for(auto worker : Workers){
        if(worker->getStatus()=="Idle")
            return false;
    }
    return true;
}

bool Stage::addNewCar(Car* newCar){
    if(this->allWorkersAreBusy()){
        queue.push_back(newCar);
        newCar->setStatus("In Line: Queue " + to_string(id));
        return true;
    }
    else{
        for(auto worker:Workers){
            if(worker->getStatus() == "Idle"){
                assignCarToWorker(worker,newCar);
                break;
            }
        }
    }
    return false;
}

vector<Stage*> getStageFromFile(const string& stagesPath , Time* time){
    vector<Stage*> result;
    ifstream stagesFile(stagesPath); 
    string header;
    getline(stagesFile, header);
    string line;
    while (getline(stagesFile, line)) {
        istringstream iss(line);
        string id_str, price_str;
        getline(iss, id_str, ',');
        getline(iss, price_str, ',');
        int price = stoi(price_str);
        int id = stoi(id_str);
        result.push_back(new Stage(id , price , time));
    }
    return result;
}

vector<Worker*> getWorkersFromFIle(const string& workersPath){
    vector<Worker*> result;
    ifstream stagesFile(workersPath); 
    string header;
    getline(stagesFile, header);
    string line;
    while (getline(stagesFile, line)) {
        istringstream iss(line);
        string id_str, Stage_id_str , Time_to_finish_str;
        getline(iss, id_str, ',');
        getline(iss, Stage_id_str, ',');
        getline(iss, Time_to_finish_str, ',');
        int id = stoi(id_str);
        int Stage_id = stoi(Stage_id_str);
        int Time_to_finish = stoi(Time_to_finish_str);
        result.push_back(new Worker(id , Stage_id , Time_to_finish));
    }
    return result;
}

vector<Car*> mergeCars(vector<Car*> firstCars, vector<Car*> secondCars){
    for(Car* car : secondCars){
        firstCars.push_back(car);
    }
    return firstCars;
}


bool compareCars(Car* car1, Car* car2) {car1->getID() < car2->getID();}

void CarWashManager::passTime(int passTime){
    for(int i=0 ; i <passTime ; i++){
        time->increaseTime();
        vector<Car*> doneCars;
        for(auto stage : stages){
            vector<Car*> stageDoneCars = stage->increaseTime();
            doneCars = mergeCars(doneCars, stageDoneCars);
        }
        sort(doneCars.begin(), doneCars.end(), compareCars);
        for(Car* car : doneCars){
            int currentStageID = car->currentStage();
            car->moveToNextStage();
            if(car->getStatus() == "Done"){
                cout << to_string(time->getTime()) + " car " + to_string(car->getID()) + ": Stage "+to_string(currentStageID) +" -> Done" << endl ;
                continue;
            }
            int nextStageID = car->currentStage();
            Stage* nextStage = findStageById(nextStageID);
            bool movedToQueue = nextStage->addNewCar(car);
            if(movedToQueue){
                cout << to_string(time->getTime()) + " car " + to_string(car->getID()) + ": Stage "+to_string(currentStageID) +" -> Queue " + to_string(nextStageID) << endl;
            } else{
                cout << to_string(time->getTime()) + " car " + to_string(car->getID()) + ": Stage "+to_string(currentStageID) +" -> Stage " + to_string(nextStageID) << endl;
            }
        }
    }
}

bool compareWorkers (Worker* i,Worker* j) { return (i->getTimeToFinish()<j->getTimeToFinish()); }

CarWashManager::CarWashManager(string stagesPath , string workersPath , Time* _time){
    stages = getStageFromFile(stagesPath, _time); 
    workers = getWorkersFromFIle(workersPath);
    sort(workers.begin(), workers.end(), compareWorkers);
    for(auto worker : workers){
        stages[worker->getStageID()-1]->addWorker(worker);
    }
    time = _time;
}

Worker* CarWashManager::findWorkerById(int workerID){
    for(int i=0 ; i < workers.size() ; i++){
        if(workers[i]->getId() == workerID){
            return workers[i];
        }
    }
    cout << "NOT FOUND" << endl;
    return 0;
}

Car* CarWashManager::findCarByID(int carID){
    for(int i=0 ; i < cars.size() ; i++){
        if(cars[i]->getID() == carID){
            return cars[i];
        }
    }
    cout << "NOT FOUND" << endl;
    return 0;
}

Stage* CarWashManager::findStageById(int stageID){
    for(int i=0; i < stages.size() ; i++){
        if(stages[i]->getId() == stageID){
            return stages[i];
        }
    }
    cout << "NOT FOUND" << endl;
    return 0;
}

void CarWashManager::get_worker_status(int workerID){
    Worker* worker = findWorkerById(workerID);
    string status = worker->getStatus();
    if(status == "Working"){
        status = status + " " + to_string(worker->getBeingWashedCar()->getID());
    }
    cout << status << endl;
}

void CarWashManager::get_stage_status(int stageID){
    findStageById(stageID)->showStatus();
}

void CarWashManager::get_car_status(int carID){
    cout<<findCarByID(carID)->getStatus()<<endl;
}

void CarWashManager::addCarToCarWash(vector<int> stages){
    Car* newCar = new Car(cars.size()+1 , stages);
    cars.push_back(newCar);
    bool movedToQueue = findStageById(newCar->getStages()[0])->addNewCar(newCar);
    if(!movedToQueue)
        cout << to_string(time->getTime()) + " car " + to_string(newCar->getID()) + ": Arrived -> Stage " + to_string(newCar->getStages()[0]) << endl ;
    else
        cout << to_string(time->getTime()) + " car " + to_string(newCar->getID()) + ": Arrived -> Queue " + to_string(newCar->getStages()[0]) << endl ;
}

vector<int> splitStringToInt(string& input, char delimiter) 
{ 
    vector<int> result;
    istringstream stream(input);  
    string token; 
    while (getline(stream, token, delimiter)) { 
        if(token!="")
            result.push_back(atoi(token.c_str())); 
    }
    return result;
} 


int main(int argc, char *argv[]){
    Time* time = new Time();
    CarWashManager carWashManager("Stages.csv", "Workers.csv", time);
    string command;
    while(cin >> command){
        if(command == "pass_time"){
            int number_of_time_units_to_pass;
            cin >> number_of_time_units_to_pass;
            carWashManager.passTime(number_of_time_units_to_pass);
        }
        else if(command == "car_arrival"){
            string stagesStr;
            getline(std::cin, stagesStr);
            vector<int> stages = splitStringToInt(stagesStr, ' ');
            carWashManager.addCarToCarWash(stages);
        }
        else if(command == "get_stage_status"){
            int stageID;
            cin >> stageID;
            carWashManager.get_stage_status(stageID);
        }
        else if(command == "get_worker_status"){
            int workerID;
            cin >> workerID;
            carWashManager.get_worker_status(workerID);
        }
        else if(command == "get_car_status"){
            int carID;
            cin >> carID;
            carWashManager.get_car_status(carID);
        }
    }
}
