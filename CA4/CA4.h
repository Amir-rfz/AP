#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Stage;

class Car{
private:
    int id;
    int currentStageIndex;
    vector<int> stages;
    string status;
public:
    Car(int _id , vector<int> _stages){id = _id; stages = _stages; currentStageIndex = 0;};
    int getID(){return id;}
    vector<int> getStages(){return stages;}
    void moveToNextStage();
    void setStatus(string _status);
    int currentStage(){return stages[currentStageIndex];};
    string getStatus(){return status;}
};

class Time{
private:
    int time;
public:
    Time(){time = 0;};
    int getTime(){return time;};
    void increaseTime();
};
void Time::increaseTime(){
    time += 1;
}

class Stage;

class Worker{
private:
    int id;
    int stageId;
    int timeToFinish;
    int appendTime;
    Car* beingWashedCar;
    string status;
public:
    Worker(int _id,int _stagedId ,int _timeToFinish);
    int getStageID(){return stageId;}
    int getId(){return id;};
    string getStatus();
    Car* goToNextTime();
    void getNewCar(Car* car){beingWashedCar = car; status="Working" ; appendTime = 0;}
    void setStatus(string _status);
    int getTimeToFinish(){return timeToFinish;};
    Car* getBeingWashedCar(){return beingWashedCar;};
};


class Stage{
private:
    int id;
    int price;
    int washedCar;
    int carsBeingWashed;
    int income;
    vector<Car*> queue;
    vector<Worker*> Workers;
    Time* time;
public:
    Stage(int _id , int _price ,Time* _time);
    int getId(){return id;}
    void passTimeInStage(int passTime);
    bool addNewCar(Car* newCar);
    void showStatus();
    void addWorker(Worker* worker);
    vector<Car*> increaseTime();
    void checkStatus();
    void addOneCarWashed();
    void choseFromQueue();
    bool allWorkersAreBusy();
    void assignCarToWorker(Worker* worker, Car* car);
};

class CarWashManager{
private:
    vector<Stage*> stages;
    vector<Worker*> workers;
    vector<Car*> cars;
    Time* time;
public:
    CarWashManager(string stagesPath , string workersPath , Time* _time);
    Stage* findStageById(int stageID);
    Worker* findWorkerById(int workerID);
    Car* findCarByID(int carID);
    void get_stage_status(int stageID);
    void get_worker_status(int workerID);
    void get_car_status(int carID);
    void addCarToCarWash(vector<int> stages);
    void passTime(int number_of_time_units_to_pass);
};


int manage(int argc, char *argv[]);