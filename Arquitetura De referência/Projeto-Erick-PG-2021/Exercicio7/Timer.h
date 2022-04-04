#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

class Timer
{
public:
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       																								  //
//  Esta função não cronometra tempo real, e sim incrementos que podem ser chamados	a qualquer momento.   //
//  Para um timer que roda junto com o loop, deve-se colocar a função tick() ao final do loop.			  //
//																										  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Timer que cronometra conforme uso da função tick(),
	Timer(int initial_time) { initialTime = initial_time; remainingTime = initial_time; }
	Timer() {}
	~Timer() {}

	//Seta o tempo inicial - Não é necessário sempre, pois já foi colocado no construtor
	inline void setInitialTime(int time) { initialTime = time; }

	//Seta o tempo restante - Não é necessário sempre, pois isto é calculado dentro de outras funções
	inline void setRemainingTime(int time) { remainingTime = time; }

	//Retorna InitialTime
	inline int getInitialTime() { return initialTime; }

	//Retorna RemainingTime
	inline int getRemainingTime() { return remainingTime; }

	//Função principal, deve ser chamada sempre que quiser passar 1 unidade de tempo
	inline void tick() { if (!over()) { remainingTime--; } }

	//Função principal, deve ser chamada sempre que quiser passar X unidades de tempo
	inline void tick(int time) { if (!over()) { remainingTime -= time; } }

	//Retorna se o timer já terminou
	inline bool over() { if (remainingTime <= 0) { return true; } else { return false; } }

	//Recomeça o timer com o parametro informado na primeira vez
	inline void restart() { remainingTime = initialTime; }

	//Recomeça o timer com novo valor de tempo
	inline void restart(int time) { remainingTime = time; initialTime = time; }


protected:
	int initialTime, remainingTime;

};

