#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class CarProcess {
private:
    int carId;
    int workerId;
    int stageId;
    int startTime;
    int finishTime;
    bool inQueue;
public:
    CarProcess(int _carID, int _workerID, int _stageID, int _startTime, int _finishTime) {
        carId = _carID; workerId = _workerID; stageId = _stageID; startTime = _startTime; finishTime = _finishTime;
    };
    int getCarID() {return carId;};
    int getWorkerID() {return workerId;};
    int getStartTime() {return startTime;};
    int getFinishTime() {return finishTime;};
    int getStageID() {return stageId;};
    bool isInQueue() {return inQueue;};

};




class CarWashRequest{
private:
    int id;
    vector<int> stages;
    int enterTime;
    vector<CarProcess*> carProcesses;
public:
    CarWashRequest(int _id , vector<int> _stages, int _enterTime){id = _id; stages = _stages; enterTime = _enterTime;};
    int getID(){return id;};
    vector<int> getStages() {return stages;};
    int findStageIndex(int stageID);
    void addCarProcesses(CarProcess* carProcess){carProcesses.push_back(carProcess);};
    string printTransitionTo(int stageID);
    string printTransitionFrom(int stageID);
    vector<CarProcess*> getCarProcesses() {return carProcesses;};
    int CarWashRequest::getCarNextStage(int currentTime);
};

int CarWashRequest::getCarNextStage(int currentTime){
    vector<CarProcess*> alreadyDoneStages = getCarProcesses();
    int lastDoneStageID = -1;
    if(!alreadyDoneStages.empty()){
        CarProcess* lastCarProcess = alreadyDoneStages.back();
        if (lastCarProcess->getFinishTime() > currentTime) {
            return -1;
        } else {
            lastDoneStageID = lastCarProcess->getStageID();
        }
    }
    int lastDoneStageIndex = findStageIndex(lastDoneStageID);
    if(lastDoneStageIndex == stages.size()-1){
        return -1;
    }
    return stages[lastDoneStageIndex+1];
}


int CarWashRequest::findStageIndex(int stageID){
    
    for( int i=0; i<stages.size(); i++) {
        if(stages[i]==stageID){
            return i;
        }
    }
    return -1; // -1 for done
}

string CarWashRequest::printTransitionTo(int stageID) {
    int stageIndex = findStageIndex(stageID);
    if(stageIndex==-1){
        return "";
    }
    CarProcess* currentCarProcess = carProcesses[stageIndex];
    CarProcess* prevCarProcess = NULL;
    if(stageIndex>0){
        prevCarProcess == carProcesses[stageIndex-1];
    }
    if(prevCarProcess==NULL){
        if(currentCarProcess->getStartTime() == enterTime){
            return "Arrived -> Stage " + to_string(stageID);
        } else {
            return "Arrived -> Queue " + to_string(stageID);
        }
    } else {
        if(currentCarProcess->getStartTime() == prevCarProcess->getFinishTime()){
            return "Stage" + to_string(prevCarProcess->getStageID()) + " -> Stage " + to_string(stageID);
        } else {
            return "Stage" + to_string(prevCarProcess->getStageID()) + " -> Queue " + to_string(stageID);
        }
    }

}

string CarWashRequest::printTransitionFrom(int stageID) {
    int stageIndex = findStageIndex(stageID);
    
    if(stageIndex == -1){
        return "";
    }
    CarProcess* currentCarProcess = carProcesses[stageIndex];

    CarProcess* nextCarProcess = NULL;
    if(stageIndex+1<carProcesses.size()){
        nextCarProcess = carProcesses[stageIndex+1];
    }

    if(nextCarProcess==NULL){
        if(stageIndex==stages.size()-1){
            return "Stage " + to_string(stageID) + " -> Done ";
        } else {
            return "Stage " + to_string(stageID) + " -> Queue " + to_string(stages[stageIndex+1]);
        }
    } else {
        if(currentCarProcess->getFinishTime() == nextCarProcess->getStartTime()){
            return "Stage" + to_string(currentCarProcess->getStageID()) + " -> Stage " + to_string(nextCarProcess->getStageID());
        } else {
            return "Stage" + to_string(currentCarProcess->getStageID()) + " -> Queue " + to_string(nextCarProcess->getStageID());
        }
    }

    

}


class Stage;

class Worker{
private:
    int id;
    int stageId;
    int timeToFinish;
public:
    Worker(int _id,int _stagedId ,int _timeToFinish);
    int getID(){return id;};
    int getStageID(){ return stageId;};
    int getTimeToFinish() { return timeToFinish; }
};


class Stage{
private:
    int id;
    int price;
public:
    Stage(int _id , int _price);
    int getID(){return id;};
};


Worker::Worker(int _id,int _stagedId ,int _timeToFinish){
    id = _id;
    stageId = _stagedId;
    timeToFinish = _timeToFinish;
}


Stage::Stage(int _id , int _price){
    id = _id;
    price = _price;
}

vector<Stage*> getStageFromFile(const string& stagesPath){
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
        result.push_back(new Stage(id , price));
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

class CarWashManager{
private:
    vector<Stage*> stages;
    vector<Worker*> workers;
    int currentTime;
    vector<CarWashRequest*> cars;
    vector<CarProcess*> carProcesses;
public:
    CarWashManager(string stagesPath , string workersPath , int currentTime);
    void processCurrentQueue();
    Stage* findStageById(int stageID);
    Worker* findWorkerById(int workerID);
    void get_stage_status(int stageID);
    void get_worker_status(int workerID);
    void get_car_status(int carID);
    void addCarToCarWash(vector<int> stages);
    void passTime(int number_of_time_units_to_pass);
    void processNewCarRequest(CarWashRequest carRequest);
    Worker* findBestAvailableWorker(int stageID);
    string getCarCurrenPosition(int carID);
    CarWashRequest* CarWashManager::findCarByID(int carID);
    int CarWashManager::getCarStageAfter(int stageID);
    vector<CarProcess*> CarWashManager::getCarProcessByWorkerID(int workerID);
    void printEventBetween(int, int);
};

void CarWashManager::passTime(int passTime){
    for(int i=0 ; i <passTime ; i++){
        currentTime += 1;
        processCurrentQueue();
    }
    printEventBetween(currentTime-passTime, currentTime);
}


void CarWashManager::printEventBetween(int startTime, int finishTime){
    for(int i=startTime ; i<=finishTime ; i++){
        for(auto carProcess : carProcesses){
            if(carProcess->getStartTime() == i){
                CarWashRequest* car = findCarByID(carProcess->getCarID());
                cout<<i<<" car "<<carProcess->getCarID()<<": "<< car->printTransitionTo(carProcess->getStageID());
            }
            if(carProcess->getFinishTime()== i){
                CarWashRequest* car = findCarByID(carProcess->getCarID());
                cout<<i<<" car "<<carProcess->getCarID()<<": "<< car->printTransitionFrom(carProcess->getStageID());
            }
        }
    }
}

CarWashManager::CarWashManager(string stagesPath , string workersPath , int _currentTime){
    stages = getStageFromFile(stagesPath); 
    workers = getWorkersFromFIle(workersPath);
    currentTime = _currentTime;
}

Worker* CarWashManager::findWorkerById(int workerID){
    for(Worker* worker : workers){
        if(worker->getID() == workerID) {
            return worker;
        }
    }
    return NULL;
}

CarWashRequest* CarWashManager::findCarByID(int carID){
    for(CarWashRequest* car : cars){
        if(car->getID() == carID) {
            return car;
        }
    }
    return NULL;
}


Worker* CarWashManager::findBestAvailableWorker(int stageID){
    Worker* bestWorker = NULL;
    int bestWorkerFinishTime = -1;
    for(Worker* worker : workers){
        if(worker->getStageID() == stageID){
            vector<CarProcess*> allCarProcesses = getCarProcessByWorkerID(worker->getID());
            if(allCarProcesses.empty() || allCarProcesses.back()->getFinishTime()<=currentTime){
                if(bestWorkerFinishTime == -1 || worker->getTimeToFinish() < bestWorkerFinishTime){
                    bestWorker = worker;
                    bestWorkerFinishTime = worker->getTimeToFinish();
                }
            }
        }
    }
    return bestWorker;
}



Stage* CarWashManager::findStageById(int stageID){
    for(int i=0; i < stages.size() ; i++){
        if(stages[i]->getID() == stageID)
            return stages[i];
        else
            cout << "NOT FOUND";
    }
    return 0;
}


void CarWashManager::processNewCarRequest(CarWashRequest carRequest){
    
    
}

vector<CarProcess*> CarWashManager::getCarProcessByWorkerID(int workerID){
    vector<CarProcess*> result;
    for(CarProcess* carProcess : carProcesses) {
        if(carProcess->getWorkerID() == workerID) {
            result.push_back(carProcess);
        }
    }
    return result;
}

void CarWashManager::processCurrentQueue(){
    for(auto car : cars) { // iterate with car orders
        int nextStageID = car->getCarNextStage(car->getID());
        if (nextStageID != -1){
            Stage* nexStage = findStageById(nextStageID);
            Worker* bestAvailableWorker = findBestAvailableWorker(nextStageID);
            if (bestAvailableWorker != NULL) {
                CarProcess* newCarProcess = new CarProcess(
                    car->getID(), bestAvailableWorker->getID(),
                    nextStageID, currentTime, 
                    currentTime+bestAvailableWorker->getTimeToFinish()
                    );
                carProcesses.push_back(newCarProcess);
                car->addCarProcesses(newCarProcess);
            }
        }
    }
}

void CarWashManager::get_worker_status(int workerID){
    cout << findWorkerById(workerID)->getStatus() << endl;
}

void CarWashManager::get_stage_status(int stageID){
    findStageById(stageID)->showStatus();
}

void CarWashManager::addCarToCarWash(vector<int> stages){
    CarWashRequest* newCar = new CarWashRequest(cars.size()+1 , stages, currentTime);
    cars.push_back(newCar);
    processCurrentQueue();
    cout << currentTime << " Car " << newCar->getID() << newCar->printTransitionTo(newCar->getStages()[0]) << endl;
}




int main(int argc, char *argv[]){
    CarWashManager carWashManager("Stages.csv", "Workers.csv", 0);
    string command;
    while(cin >> command){
        if(command == "pass_time"){
            int number_of_time_units_to_pass;
            cin >> number_of_time_units_to_pass;
            carWashManager.passTime(number_of_time_units_to_pass);
        }
        else if(command == "car_arrival"){
            vector<int> stages;
            //TODO: Get stages
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