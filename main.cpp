#include <iostream>   // std::cout
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>    // std::transform

using namespace std;

vector<double> sense(const vector<double> p, const vector<string> world, 
	const string measurement, const double phit, const double pmiss)
{
	vector<double> q;  // use to return vector
	for (size_t i = 0; i < p.size(); i++)
	{
		auto hit = (measurement == world[i]);
		q.push_back(p[i] * (hit * phit + (1 - hit) * pmiss));
	}
	
	double q_sum = accumulate(q.begin(), q.end(), 0.0);
	// divide each element in q vector by sum

	transform(q.begin(), q.end(), q.begin(), 
		bind(divides<double>(), placeholders::_1, q_sum));
	
	return q;
}
vector<double> my_move(const vector<double> p, const int motion,
	double pExact, double pOvershoot, double pUndershoot)
{
	vector<double> q;  // use to return vector
	for (size_t i = 0; i < p.size(); i++)
	{
		auto s = pExact * p[(i - motion) % p.size()];
		s = s + pOvershoot * p[(i - motion - 1) % p.size()];
		s = s + pUndershoot * p[(i - motion + 1) % p.size()];
		q.push_back(s);
	}
	return q;
}

int main()
{
	vector<double> main_p = { 0.2, 0.2, 0.2, 0.2, 0.2 };
	vector<string> main_world = { "green", "red", "red", "green", "green" };
	vector<string> main_measurements = { "red", "green" };
	vector<int> main_motions = { 1, 1 };
	double main_pHit = 0.6;
	double main_pMiss = 0.2;
	double main_pExact = 0.8;
	double main_pOvershoot = 0.1;
	double main_pUndershoot = 0.1;
	
	for (size_t i = 0; i < main_measurements.size(); i++)
	{
		main_p = sense(main_p, main_world, main_measurements[i],
				main_pHit, main_pMiss);
		
		main_p = my_move(main_p, main_motions[i], main_pExact, main_pOvershoot,
			main_pUndershoot);
	}
	cout << "p is ";
	for (auto p : main_p)
	{
		cout << "   " << p;
	}
	cout << endl;
	
	return 0;
}

