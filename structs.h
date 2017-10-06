#ifndef STRUCTS_H
#define STRUCTS_H

# define COMPRESS_LEVEL 6

typedef struct {
    int     v1;
	char*   st1;
} Gen;


typedef struct {
    int     v1;
	char*   st1;
    int     v2;
	char*   st2;
} SpecExt;

typedef struct {
	long 	id;
	short	x;
	short 	y;
	float 	energy;
	short   age;
	short   maxage;
	float   speed;
	float   distEvol;
	short   actionOffset;
	int     specId;
	short 	reprodFail1;
	float 	reprodFail2;
	float 	reprodFail3;
	float 	reprodFail4;
	float 	reprodFail5;
	short 	trans;

}CurrIndv;

typedef struct {
	long 	id;
	short	x;
	short 	y;
	float 	energy;
	short   age;
	int     specId;
	short 	trans;

}CurrIndv_C;

typedef struct {
    long    id;
    short   x;
    short   y;
    float   energy;
    short   age;
    short   maxage;
	float   speed;
	float   distEvol;
	int     specId;
	int     specP1Id;
	int     specP2Id;
	long     parent1Id;
	long     parent2Id;
	short    gender;
}NewIndv;

typedef struct {
    long     id;
    short   x;
    short   y;
    float   energy;
    short   age;
	int     specId;
	int     specP1Id;
	int     specP2Id;
	long     parent1Id;
	long     parent2Id;
	short    gender;
}NewIndv_C;

typedef struct {
	long     id;
    short   x;
    short   y;
    float   energy;
    short   age;
    short   maxage;
	float   speed;
	float   distEvol;
	short   actionOffset;
	short   lastAction;
	int     specId;
	int     specP1Id;
	int     specP2Id;
	long     parent1Id;
	long     parent2Id;
	short   conceptIndex;
	float   conceptMultiplier;
	short   gender;
	short persuasion;
}Indv;

typedef struct {
    int     specId;
    long     id;
    short   deathType;
    short   lastAction;
    short   age;
    float   distEvol;
	float   energy;
    float   speed;
}DeadIndv;

#endif // STRUCTS_H
