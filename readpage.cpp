#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;
class launch {
    public:
        int launchId;
        string name;
        string rocket;
        string location;
        string date;
        string utc;
        string time;
        string imageUrl;
        string description;
        string window;
};

int getLaunchNumbers(int &launches, string HTML);
int getLaunchInfo(int launchnumber, launch &launchinfo, string HTML);
string getTableFromHTML(string fileName, bool reload);


int main(){
    string HTML = getTableFromHTML("html",1);
    int launches[100] = {};
    getLaunchNumbers(launches[0],HTML);

    int i = 0;
    while (launches[i] != 0){
        cout << launches[i] << endl;launch falcon;
        getLaunchInfo(launches[i],falcon,HTML);

        cout << falcon.date << endl;
        cout << falcon.name << endl;
        cout << falcon.rocket << endl;
        cout << falcon.imageUrl << endl;
        cout << falcon.description << endl;
        cout << falcon.location << endl;
        cout << falcon.time << endl;
        cout << falcon.utc << endl;
        cout << falcon.window << endl;

        i++;
    }
}


int getLaunchNumbers(int &launches, string HTML) { //this function stores the launch numbers in an array
    //extract the html table from the file
    //string HTML = getTableFromHTML("html");

    //get the launch numbers
    int loc = 0;
    int launchnum = 0;
    while((loc = (int)HTML.find("\" id=\"launch-",loc+1)) != -1) { //as long as there is a next launch store it's location in the string as loc
        char num;
        int i = 0;
        int launch = 0;
        string launchstring = "";
        //get each launch number and store it as a string
        while((num = (int)(HTML[loc+13+i])) != 34) {
            launchstring += num;
            i++;
        }
        //convert this string to an int
        for(int i = 0; i < launchstring.length(); i++){
            launch += ((int)launchstring[i] - 48) * pow(10,launchstring.length()-i-1);
        }
        //store this int in the array
        *(launchnum + &launches) = launch;
        launchnum++;
    }
}


string getTableFromHTML(string fileName,bool reload){ //read the file and return table part of html
    //reload the html page if reload is 1
    if (reload){
        system("curl http://www.spaceflightinsider.com/launch-schedule/ > html");
    }
    
    //read file and store as string HTML
    ifstream file(fileName);  
    stringstream buffer;
    buffer << file.rdbuf();
    string HTML = buffer.str();

    //take the table out of the HTML
    string temp = "";
    for (int i = HTML.find("<table class=\"launchcalendar"); i < HTML.find("<div class=\"news-navigation\""); i++){
        temp += HTML[i];
    }
    return temp;
}

int getLaunchInfo(int launchid, launch &launchinfo, string HTML){
    //extract the html table from the file
    //string HTML = getTableFromHTML("html");
    string temp = "";
    for (int i = HTML.find("id=\"launch-" + to_string(launchid)); i < HTML.find("</table>",HTML.find("</p>",HTML.find("id=\"launch-" + to_string(launchid)))); i++){
        temp += HTML[i];
    }
    HTML = temp;

    launchinfo.launchId = launchid;
    
    for (int i = HTML.find("<th colspan=\"2\">") + 16; i < HTML.find("</th>",HTML.find("<th colspan=\"2\">")); i++){
        launchinfo.name += HTML[i];
    }

    for (int i = HTML.find("<br") + 14; i < HTML.find("</td>") - 7; i++){
        launchinfo.rocket += HTML[i];
    }

    for (int i = HTML.find("<td>",HTML.find(">Location<")) + 4; i < HTML.find("</td>",HTML.find("<td>",HTML.find(">Location<"))); i++){
        launchinfo.location += HTML[i];
    }

    if ((int)HTML.find("<th>Window</th>") != -1){
        for (int i = HTML.find("<td>",HTML.find(">Window<")) + 4; i < HTML.find("</td>",HTML.find("<td>",HTML.find(">Window<"))); i++){
            launchinfo.window += HTML[i];
        }
    }

    for (int i = HTML.find("<span>") + 6; i < HTML.find("</span>",HTML.find("<span>")); i++){
        launchinfo.date += HTML[i];
    }

    for (int i = HTML.find("utc\"> /",HTML.find(">Time<")) + 8; i < HTML.find("</span>",HTML.find("utc\">",HTML.find(">Time<"))); i++){
        launchinfo.utc += HTML[i];
    }
    if (launchinfo.utc.length() > 50) {
        launchinfo.utc = "";
    }

    for (int i = HTML.find("<td>",HTML.find(">Time<")) + 4; i < HTML.find("<span class=\"",HTML.find("<td>",HTML.find(">Time<"))); i++){
        launchinfo.time += HTML[i];
    }

    for (int i = HTML.find("<div style=") + 35; i < HTML.find("</div>") - 5; i++){
        launchinfo.imageUrl += HTML[i];
    }

    for (int i = HTML.find("<p>") + 3; i < HTML.find("</p>"); i++){
        launchinfo.description += HTML[i];
    }
}
