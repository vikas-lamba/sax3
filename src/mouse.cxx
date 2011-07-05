#include<iostream>
#include<vector>
#include<utility>
#include<fstream>

extern "C"{
#include<augeas.h>
}

#include "ui/yuifactory.h"

using namespace std;

class Mouse{

	class Details{
		string product;
		string vendor;
		string name;
		string protocol;
		string device;
		public:
		void setProduct(string);
		void setVendor(string);
		void setName(string);	
		void setDevice(string);
		Details();
	};
	
	string fileName,line;
	vector<Details*> d;

	UI::YUIFactory * factory;
	UI::yDialog *dialog;
	UI::yHLayout * hl1,*enableButton3Layout,*enableWheelLayout;
	UI::yVLayout * vl1;
	UI::yPushButton * okButton,*cancelButton;
	UI::yRadioButtonGroup * button3,*wheel;
	UI::yLabel * button3Label,*enableButton3Label,*wheelLabel,*enableWheelLabel;
	UI::yIntField * timeout,*wheeltimeout;
	void getProductVendor();
	void getName();
	public:
	void autodetect();
	void initUI();
	Mouse();
};

void Mouse::Details::setProduct(string p){
	product = p;
}

void Mouse::Details::setVendor(string v){
	vendor = v;
}

void Mouse::Details::setName(string n){
	name = n;
}

void Mouse::Details::setDevice(string d){
	device = d;
}

Mouse::Details::Details(){
	protocol = "Auto";
}

void Mouse::getProductVendor(){
	string pv = line.substr(line.find_first_of('/')+1,line.find_last_of('/'));
	string v = pv.substr(0,pv.find_first_of('/'));
	d.back()->setVendor(v);
	pv.erase(0,pv.find_first_of('/')+1);
	string p = pv.substr(0,pv.find_first_of('/'));
	d.back()->setVendor(p);
}

void Mouse::getName(){
	int x = line.find_last_of('"')-6;
	string n = line.substr(line.find_first_of('"')+1,x);
	d.back()->setName(n);
}

void Mouse::autodetect(){
	fileName = "/sys/class/input/mouse0/device/uevent";
	ifstream myfile;
	string::iterator it;
	
	for(int i=0;i<=9;i++){
		fileName.replace(fileName.begin()+22,fileName.begin()+23,1,'0'+i);
		myfile.open(fileName.c_str());
		if(myfile.is_open()){
			d.push_back(new Details());
			string device = "/dev/mouse";device.push_back('0'+i);
			d.back()->setDevice(device);
			while(myfile.good()){
				getline(myfile,line);
				if(!line.find("PRODUCT"))
					getProductVendor();
				if(!line.find("NAME"))
					getName();
			}
		}
		myfile.close();
	
	}
}

void Mouse::initUI(){
	dialog = factory->createDialog(30,10);

	vl1 = factory->createVLayout(dialog);
	hl1 = factory->createHLayout(vl1);

	button3Label = factory->createLabel(vl1,"3 button Emulation Options");
	enableButton3Layout = factory->createHLayout(vl1);
	enableButton3Label = factory->createLabel(enableButton3Layout,"Enable 3 Buttons");
	button3 = factory->createRadioButtonGroup(enableButton3Layout);
	button3->addButton("Yes");
	button3->addButton("No");
	timeout = factory->createIntField(vl1,"3 Button Timeout Time",0,100,50);

	wheelLabel = factory->createLabel(vl1,"Wheel Emulation Options");
	enableWheelLayout = factory->createHLayout(vl1);
	enableWheelLabel = factory->createLabel(enableWheelLayout,"Enable Wheel Emulation");
	wheel = factory->createRadioButtonGroup(enableWheelLayout);
	wheel->addButton("Yes");
	wheel->addButton("No");
	wheeltimeout = factory->createIntField(vl1,"3 Button Timeout Time",0,100,50);

	dialog->wait();
}

Mouse::Mouse(){
	factory = new UI::YUIFactory();
}

int main(){
	Mouse * m = new Mouse();
	m->autodetect();
	m->initUI();
	return 0;
}