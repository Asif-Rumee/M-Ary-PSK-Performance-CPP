/*
Design of modulator and demodulator of M-ary PSK over AWGN channel.
Author: Asif Rahman Rumee
Date: 30/12/2021
*/

#include<iostream>
#include<complex>
#include<vector>
#include<cmath>
#include<math.h>
#include<ctime>

#define PI 3.141592653589

using namespace std;

class mpsk_modem {
public:
	vector<double> points;
	vector<double> I, Q;
	vector<complex<double>> constellation;

	mpsk_modem(short total_point, short of = 1) {
		for (short i = 0; i<total_point; i++) {
			points.push_back(i);
			I.push_back(of * 1 / sqrt(2)*cos(points[i] / total_point * 2 * PI));
			Q.push_back(of * 1 / sqrt(2)*sin(points[i] / total_point * 2 * PI));
			constellation.push_back(complex<double>(I[i], Q[i]));
		}
	}

	vector<complex<double>> modulate(vector<short> input_symbols) {
		vector<complex<double>> modulated_vector;
		for (unsigned long i = 0; i<input_symbols.size(); i++) {
			modulated_vector.push_back(constellation[input_symbols[i]]);
		}
		return modulated_vector;
	}

	vector<short> demodulate(vector<complex<double>> received_symbols) {
		vector<short> detected_symbols;
		for (unsigned long i = 0; i<received_symbols.size(); i++) {
			detected_symbols.push_back(min_distance_detector(received_symbols[i]));
		}
		return detected_symbols;
	}

	double distance(complex<double> p, complex<double> q) {
		return abs(p - q);
	}

	short min_distance_detector(complex<double> received_symbol) {
		short min_idx = 0;
		long min_distance = LONG_MAX;
		for (short i = 0; i<constellation.size(); i++) {
			if (min_distance > distance(constellation[i], received_symbol)) {
				min_idx = i;
				min_distance = distance(constellation[i], received_symbol);
			}
		}
		return min_idx;
	}

	vector<complex<double>> awgn(vector<complex<double>> signal_elements, int snr_db, int wsf = 1) {
		double snr = pow(10, snr_db / 10);
		long double power = 0;
		for (unsigned long i = 0; i<signal_elements.size(); i++) {
			power += abs(signal_elements[i]);
		}
		power = wsf*power / signal_elements.size();
		double N0 = power / snr;
		double nm = sqrt(N0 / 2);
		srand((unsigned)time(NULL));
		vector<complex<double>> received_signal;
		for (unsigned long i = 0; i<signal_elements.size(); i++) {
			complex<double> r(real(signal_elements[i]) + (nm*(float)rand() / RAND_MAX), imag(signal_elements[i]) + (nm*(float)rand() / RAND_MAX));
			received_signal.push_back(r);
		}
		return received_signal;
	}
};

