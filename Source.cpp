/*
Performance analysis of M-ary PSK over AWGN channel by BER vs Eb/N0 chart.
Author: Asif Rahman Rumee
Date: 30/12/2021
*/

#include<vector>
#include<cmath>
#include<iostream>
#include<random>

#include "mpsk_modem.cpp"
#include "gnuplot-iostream.h"

short bit_differences(short symbol_a, short symbol_b){
	short XOR = symbol_a ^ symbol_b;
	short count = 0;
	while (XOR) {
		XOR = XOR & (XOR - 1);
		count++;
	}
	return count;
}

int main() {
	random_device rd;
	mt19937 gen(rd());
	
	Gnuplot gp("\"H:\\software\\CodeBlocks\\GNUplot\\gnuplot\\bin\\gnuplot.exe\"");

	double nSym = 1000;
	vector<short> EbN0dBs = { -2, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };
	short arrayOfM[] = { 2, 4, 8, 16, 32 };
	short colors[] = { 0, 2, 4, 6, 8 };
	
	gp << "set xrange [-2:24]\n set logscale y 10\n set xlabel 'Eb/N0'\n set ylabel 'BER'\n set title 'Performance analysis of MPSK over AWGN'\n";
	for (short m = 0; m < 5; m++) {
		short nByts = log2(arrayOfM[m]);
		vector<double> EsN0dBs;
		for (short i = 0; i < EbN0dBs.size(); i++) {
			EsN0dBs.push_back(10*log10(nByts)+ EbN0dBs[i]);
		}
		uniform_int_distribution<> distr(0, arrayOfM[m]-1);
		vector<short> inputSyms;
		for (unsigned int i = 0; i < nSym; i++) {
			inputSyms.push_back(distr(gen));
		}
		cout << endl;
		mpsk_modem modem = mpsk_modem(arrayOfM[m]);
		vector<complex<double>> modulatedSyms = modem.modulate(inputSyms);
		vector<double> BER; 
		for (short j = 0; j < EsN0dBs.size(); j++) {
			vector<complex<double>> receivedSyms = modem.awgn(modulatedSyms, EsN0dBs[j]);
			vector<short> detectedSyms = modem.demodulate(receivedSyms);
			unsigned int sum = 0;
			for (unsigned int i = 0; i < detectedSyms.size(); i++) {
				if (detectedSyms[i] != inputSyms[i]) {
					sum += bit_differences(detectedSyms[i], inputSyms[i]);
				}
			}
			BER.push_back(sum / nSym*nByts);
		}

		switch (m) {
			case 0:
				gp << "point1 = " << arrayOfM[m] << endl;
				gp << "plot" << gp.file1d(BER) << "with linespoints title sprintf('M = %d', point1)" << endl;
				break;
			case 1:
				gp << "point2 = " << arrayOfM[m] << endl;
				gp << "replot" << gp.file1d(BER) << "with linespoints title sprintf('M = %d', point2)" << endl;
				break;
			case 2:
				gp << "point3 = " << arrayOfM[m] << endl;
				gp << "replot" << gp.file1d(BER) << "with linespoints title sprintf('M = %d', point3)" << endl;
				break;
			case 3:
				gp << "point4 = " << arrayOfM[m] << endl;
				gp << "replot" << gp.file1d(BER) << "with linespoints title sprintf('M = %d', point4)" << endl;
				break;
			default:
				gp << "point5 = " << arrayOfM[m] << endl;
				gp << "replot" << gp.file1d(BER) << "with linespoints title sprintf('M = %d', point5)" << endl;
		}
	}

	system("pause");
	return 0;
}
