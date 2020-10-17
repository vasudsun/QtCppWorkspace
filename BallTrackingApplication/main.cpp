#include <iostream>
#include<thread>
#include<queue>
#include<string>
#include<condition_variable>
#include<map>
using namespace std;

class BallTrackingData;
map<float,BallTrackingData>ballPoint;
mutex mtx;
condition_variable condition;





/*******************PayLoad class************************/
class BallTrackingData
{
public:

    BallTrackingData()
    {


    }

    string playnerName;
    int cameraNumber=0;
    int x_center=0,y_center=0;
    int teamPosession;
};


/****The function ProcessResults ,corrensponds to one camera
 * It will process the Time stamp, Image detection paramaters,player name details,
 * Team posession. For each camera we need to create the thread with
 * corresponsing thread Function*************/
void ProcessResults()
{

    /***Algorithm to findout the results of the Ballgame****/

    //BlockingQueue<BallTrackingData>mainqueue(5);
    BallTrackingData ballData;
    float timestamp;
    timestamp=1.752;
    ballData.x_center=0;
    ballData.y_center=10;
    ballData.playnerName="Dhawan";
    ballData.cameraNumber=1;
    ballData.teamPosession=1;

    /****Insert data to the map container after making the function thread safe**************/

    unique_lock<mutex>lock(mtx);
    cout<<"Inserting data"<<endl;
    ballPoint.insert({timestamp,ballData});//Adding item to the map which will be sorted interms of the time stamp
    lock.unlock();
    condition.notify_one();

}


/***Thread function of the consumer thread for retrieving the results
 There will be only one consumer thread which is responsible for retrieving the
results**********/

void RetrieveResults()
{

     cout<<"RetrievingResults function has been called"<<endl;

    /***Making the function thread safe***********/
    unique_lock<mutex>lock(mtx);
    // condition.wait(lock,[&](){return ballPoint.size() > 1;});
    if(ballPoint.size() > 0)
    {
        cout<<"Retrieving  data"<<endl;
    for(map<float,BallTrackingData>::iterator it=ballPoint.begin();it!=ballPoint.end();it++)
    {

        cout<<"Time stamp is --> "<<it->first<<endl;
        cout<<"Player Name---> "<<it->second.playnerName<<endl;
        cout<<"camera Number--> "<<it->second.cameraNumber<<endl;
    }
    }
    lock.unlock();
    condition.notify_one();

}

int main()
{


    /****Creating the threads for producer ,consumer threads*******/
    thread producerThread(&ProcessResults);
    thread consumerthread(&RetrieveResults);

    producerThread.join();
    consumerthread.join();



    return 0;
}
