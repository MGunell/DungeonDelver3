#pragma once
#include <vector>

//for note duration, 1 = whole, 2 = half, 4 = quarter, 8 = eigth, 16 = 16th etc...
//for accidental, 0 = flat, 1 = natural, 2 = sharp

class Note
{
public:
	Note()
	{
		duration = 4;
		tone = 'E';
		octave = 2;
		accidental = 1;
	}

	int duration;
	char tone;
	int octave;
	int accidental;
};

class Measure
{
public:
	Measure()
	{

	}
	int spot;
	std::vector<Note> notes;

	int width;
	int staffSpacing;
};

class Song
{
public:
	Song()
	{
		timeSigTop = 4;
		timeSigBottom = 4;
	}

	int timeSigTop, timeSigBottom;
	int measures = 10;
	int tempo;
	char key;
	std::string instrument;
	std::vector<Measure> measures;
};