#include <iostream>
#include <string>
#include <fstream>
#include <ampl/ampl.h>
#include <ampl/environment.h>
#include <ampl/dataframe.h>
#include <string>

using namespace std;
using namespace ampl;

//Suffixes for obtaining gap from baron
StringArgs a = { "obj_lb","obj_ub" };

void doa(AMPL& ampli) {
	double p1 = 1, p2, p3;
	for (p1 = 1; p1 >= -0.05; p1 -= 0.05) {
		p2 = 1 - p1;
		for (p2; p2 >= 0; p2 -= 0.05) {
			p3 = 1 - p1 - p2;
			//Read data:
			ampli.readData("Insert data route here");
			//Get weights from AMPL
			Parameter P1 = ampli.getParameter("P1");
			Parameter P2 = ampli.getParameter("P2");
			Parameter P3 = ampli.getParameter("P3");
			//Set weights values
			//P1.setValues(new double[] {p1}, 1);
			//P2.setValues(new double[] {p2}, 1);
			//P3.setValues(new double[] {p3}, 1);
			P1.set(p1);
			P2.set(p2);
			P3.set(p3);
			//Solve new model:
			ampli.solve();
			//Ofstream file:
			ofstream outs("ins " + to_string(p1) + " " + to_string(p2) + " " + to_string(p3) + ".txt");
			outs << "P1: " << p1 << "\nP2: " << p2 << "\nP3: " << p3 << endl;
			//Get objective functions and values:
			outs << ampli.getVariable("Z1").getValues().toString() << endl;
			outs << ampli.getVariable("Z2").getValues().toString() << endl;
			outs << ampli.getVariable("Z3").getValues().toString() << endl;
			//Get gap: 
			DataFrame obj = ampli.getObjective("functions").getValues(a);
			DataFrame::Row row = obj.getRowByIndex(0);
			double gap = 1 - row[0].dbl() / row[1].dbl();
			outs << "Gap: " << gap;
			outs.close();
			cout << "Done\n";
			//Reset data
			ampli.eval("reset data;");
		}
	}
}
int main(int argc, char** argv) {
	//Create environment and obtain AMPL installation path
	Environment env("Specify ampl root route here");
	AMPL ampli(env);
	//Set options such as solver and the solver options
	ampli.setOption("solver", "baron");
	ampli.setOption("baron_options", "maxtime=19 objbound");
	cout << *ampli.getOption("baron_options") << endl;

	//Read Model and Data
	ampli.read("Specify .mod file here");

	doa(ampli);

}