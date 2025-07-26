#include "Agent.h"

#include "settings.h"
#include "helpers.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "DWRAONBrain.h"
#include "MLPBrain.h"
#include "AssemblyBrain.h"

using namespace std;
Agent::Agent()
{
    pos= Vector2f(randf(0,conf::WIDTH),randf(0,conf::HEIGHT));
    angle= randf(-M_PI,M_PI);
    health= 1.0+randf(0,0.1);
    age=0;
    spikeLength=0;
    red= 0;
    gre= 0;
    blu= 0;
    w1=0;
    w2=0;
    soundmul=1;
    give=0;
    clockf1= randf(5,100);
    clockf2= randf(5,100);
    boost=false;
    indicator=0;
    gencount=0;
    selectflag=0;
    ir=0;
    ig=0;
    ib=0;
    temperature_preference=randf(0,1);
    hybrid= false;
    herbivore= randf(0,1);
    repcounter= herbivore*randf(conf::REPRATEH-0.1,conf::REPRATEH+0.1) + (1-herbivore)*randf(conf::REPRATEC-0.1,conf::REPRATEC+0.1);

    id=0;
    
    smellmod= randf(0.1, 0.5);
    soundmod= randf(0.2, 0.6);
    hearmod= randf(0.7, 1.3);
    eyesensmod= randf(1, 3);
    bloodmod= randf(1, 3);
    
    MUTRATE1= randf(0.001, 0.005);
    MUTRATE2= randf(0.03, 0.07);

    spiked= false;
    
    in.resize(INPUTSIZE, 0);
    out.resize(OUTPUTSIZE, 0);
    
    eyefov.resize(NUMEYES, 0);
    eyedir.resize(NUMEYES, 0);
    for(int i=0;i<NUMEYES;i++) {
        eyefov[i] = randf(0.5, 2);
        eyedir[i] = randf(0, 2*M_PI);
    }
}

void Agent::printSelf()
{
    printf("Agent age=%i\n", age);
    for (int i=0;i<mutations.size();i++) {
        cout << mutations[i];
    }
}

void Agent::initEvent(float size, float r, float g, float b)
{
    indicator=size;
    ir=r;
    ig=g;
    ib=b;
}

void Agent::tick()
{
    brain.tick(in, out);
}
Agent Agent::reproduce(float MR, float MR2)
{
    bool BDEBUG = false;
    if(BDEBUG) printf("New birth---------------\n");
    Agent a2;

    //spawn the baby somewhere closeby behind agent
    //we want to spawn behind so that agents dont accidentally eat their young right away
    Vector2f fb(conf::BOTRADIUS,0);
    fb.rotate(-a2.angle);
    a2.pos= this->pos + fb + Vector2f(randf(-conf::BOTRADIUS*2,conf::BOTRADIUS*2), randf(-conf::BOTRADIUS*2,conf::BOTRADIUS*2));
    if (a2.pos.x<0) a2.pos.x= conf::WIDTH+a2.pos.x;
    if (a2.pos.x>=conf::WIDTH) a2.pos.x= a2.pos.x-conf::WIDTH;
    if (a2.pos.y<0) a2.pos.y= conf::HEIGHT+a2.pos.y;
    if (a2.pos.y>=conf::HEIGHT) a2.pos.y= a2.pos.y-conf::HEIGHT;

    a2.gencount= this->gencount+1;
    a2.repcounter= a2.herbivore*randf(conf::REPRATEH-0.1,conf::REPRATEH+0.1) + (1-a2.herbivore)*randf(conf::REPRATEC-0.1,conf::REPRATEC+0.1);

    //noisy attribute passing
    a2.MUTRATE1= this->MUTRATE1;
    a2.MUTRATE2= this->MUTRATE2;
    if (randf(0,1)<0.1) a2.MUTRATE1= randn(this->MUTRATE1, conf::METAMUTRATE1);
    if (randf(0,1)<0.1) a2.MUTRATE2= randn(this->MUTRATE2, conf::METAMUTRATE2);
    if (this->MUTRATE1<0.001) this->MUTRATE1= 0.001;
    if (this->MUTRATE2<0.02) this->MUTRATE2= 0.02;
    a2.herbivore= cap(randn(this->herbivore, 0.03));
    if (randf(0,1)<MR*5) a2.clockf1= randn(a2.clockf1, MR2);
    if (a2.clockf1<2) a2.clockf1= 2;
    if (randf(0,1)<MR*5) a2.clockf2= randn(a2.clockf2, MR2);
    if (a2.clockf2<2) a2.clockf2= 2;
    
    a2.smellmod = this->smellmod;
    a2.soundmod = this->soundmod;
    a2.hearmod = this->hearmod;
    a2.eyesensmod = this->eyesensmod;
    a2.bloodmod = this->bloodmod;
    if(randf(0,1)<MR*5) {float oo = a2.smellmod; a2.smellmod = randn(a2.smellmod, MR2); if(BDEBUG) printf("smell mutated from %f to %f\n", oo, a2.smellmod);}
    if(randf(0,1)<MR*5) {float oo = a2.soundmod; a2.soundmod = randn(a2.soundmod, MR2); if(BDEBUG) printf("sound mutated from %f to %f\n", oo, a2.soundmod);}
    if(randf(0,1)<MR*5) {float oo = a2.hearmod; a2.hearmod = randn(a2.hearmod, MR2); if(BDEBUG) printf("hear mutated from %f to %f\n", oo, a2.hearmod);}
    if(randf(0,1)<MR*5) {float oo = a2.eyesensmod; a2.eyesensmod = randn(a2.eyesensmod, MR2); if(BDEBUG) printf("eyesens mutated from %f to %f\n", oo, a2.eyesensmod);}
    if(randf(0,1)<MR*5) {float oo = a2.bloodmod; a2.bloodmod = randn(a2.bloodmod, MR2); if(BDEBUG) printf("blood mutated from %f to %f\n", oo, a2.bloodmod);}
    
    a2.eyefov = this->eyefov;
    a2.eyedir = this->eyedir;
    for(int i=0;i<NUMEYES;i++){
        if(randf(0,1)<MR*5) a2.eyefov[i] = randn(a2.eyefov[i], MR2);
        if(a2.eyefov[i]<0) a2.eyefov[i] = 0;
        
        if(randf(0,1)<MR*5) a2.eyedir[i] = randn(a2.eyedir[i], MR2);
        if(a2.eyedir[i]<0) a2.eyedir[i] = 0;
        if(a2.eyedir[i]>2*M_PI) a2.eyedir[i] = 2*M_PI;
    }
    
    a2.temperature_preference= cap(randn(this->temperature_preference, 0.005));
//    a2.temperature_preference= this->temperature_preference;
    
    //mutate brain here
    a2.brain= this->brain;
    a2.brain.mutate(MR,MR2);
    
    return a2;

}

Agent Agent::crossover(const Agent& other)
{
    //this could be made faster by returning a pointer
    //instead of returning by value
    Agent anew;
    anew.hybrid=true; //set this non-default flag
    anew.gencount= this->gencount;
    if (other.gencount<anew.gencount) anew.gencount= other.gencount;

    //agent heredity attributes
    anew.clockf1= randf(0,1)<0.5 ? this->clockf1 : other.clockf1;
    anew.clockf2= randf(0,1)<0.5 ? this->clockf2 : other.clockf2;
    anew.herbivore= randf(0,1)<0.5 ? this->herbivore : other.herbivore;
    anew.MUTRATE1= randf(0,1)<0.5 ? this->MUTRATE1 : other.MUTRATE1;
    anew.MUTRATE2= randf(0,1)<0.5 ? this->MUTRATE2 : other.MUTRATE2;
    anew.temperature_preference = randf(0,1)<0.5 ? this->temperature_preference : other.temperature_preference;
    
    anew.smellmod= randf(0,1)<0.5 ? this->smellmod : other.smellmod;
    anew.soundmod= randf(0,1)<0.5 ? this->soundmod : other.soundmod;
    anew.hearmod= randf(0,1)<0.5 ? this->hearmod : other.hearmod;
    anew.eyesensmod= randf(0,1)<0.5 ? this->eyesensmod : other.eyesensmod;
    anew.bloodmod= randf(0,1)<0.5 ? this->bloodmod : other.bloodmod;
    
    anew.eyefov= randf(0,1)<0.5 ? this->eyefov : other.eyefov;
    anew.eyedir= randf(0,1)<0.5 ? this->eyedir : other.eyedir;
    
    anew.brain= this->brain.crossover(other.brain);
    
    return anew;
}

void Agent::saveToStream(std::ofstream& file) const
{
    // Save basic properties
    file.write(reinterpret_cast<const char*>(&pos.x), sizeof(pos.x));
    file.write(reinterpret_cast<const char*>(&pos.y), sizeof(pos.y));
    file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    file.write(reinterpret_cast<const char*>(&angle), sizeof(angle));
    file.write(reinterpret_cast<const char*>(&red), sizeof(red));
    file.write(reinterpret_cast<const char*>(&gre), sizeof(gre));
    file.write(reinterpret_cast<const char*>(&blu), sizeof(blu));
    file.write(reinterpret_cast<const char*>(&w1), sizeof(w1));
    file.write(reinterpret_cast<const char*>(&w2), sizeof(w2));
    file.write(reinterpret_cast<const char*>(&boost), sizeof(boost));
    file.write(reinterpret_cast<const char*>(&spikeLength), sizeof(spikeLength));
    file.write(reinterpret_cast<const char*>(&age), sizeof(age));
    file.write(reinterpret_cast<const char*>(&spiked), sizeof(spiked));
    
    // Save input/output vectors
    size_t inSize = in.size();
    size_t outSize = out.size();
    file.write(reinterpret_cast<const char*>(&inSize), sizeof(inSize));
    file.write(reinterpret_cast<const char*>(&outSize), sizeof(outSize));
    file.write(reinterpret_cast<const char*>(in.data()), inSize * sizeof(float));
    file.write(reinterpret_cast<const char*>(out.data()), outSize * sizeof(float));
    
    // Save other properties
    file.write(reinterpret_cast<const char*>(&repcounter), sizeof(repcounter));
    file.write(reinterpret_cast<const char*>(&gencount), sizeof(gencount));
    file.write(reinterpret_cast<const char*>(&hybrid), sizeof(hybrid));
    file.write(reinterpret_cast<const char*>(&clockf1), sizeof(clockf1));
    file.write(reinterpret_cast<const char*>(&clockf2), sizeof(clockf2));
    file.write(reinterpret_cast<const char*>(&soundmul), sizeof(soundmul));
    file.write(reinterpret_cast<const char*>(&indicator), sizeof(indicator));
    file.write(reinterpret_cast<const char*>(&ir), sizeof(ir));
    file.write(reinterpret_cast<const char*>(&ig), sizeof(ig));
    file.write(reinterpret_cast<const char*>(&ib), sizeof(ib));
    file.write(reinterpret_cast<const char*>(&selectflag), sizeof(selectflag));
    file.write(reinterpret_cast<const char*>(&dfood), sizeof(dfood));
    file.write(reinterpret_cast<const char*>(&give), sizeof(give));
    file.write(reinterpret_cast<const char*>(&id), sizeof(id));
    file.write(reinterpret_cast<const char*>(&herbivore), sizeof(herbivore));
    file.write(reinterpret_cast<const char*>(&MUTRATE1), sizeof(MUTRATE1));
    file.write(reinterpret_cast<const char*>(&MUTRATE2), sizeof(MUTRATE2));
    file.write(reinterpret_cast<const char*>(&temperature_preference), sizeof(temperature_preference));
    file.write(reinterpret_cast<const char*>(&smellmod), sizeof(smellmod));
    file.write(reinterpret_cast<const char*>(&soundmod), sizeof(soundmod));
    file.write(reinterpret_cast<const char*>(&hearmod), sizeof(hearmod));
    file.write(reinterpret_cast<const char*>(&eyesensmod), sizeof(eyesensmod));
    file.write(reinterpret_cast<const char*>(&bloodmod), sizeof(bloodmod));
    
    // Save eye properties
    size_t eyeSize = eyefov.size();
    file.write(reinterpret_cast<const char*>(&eyeSize), sizeof(eyeSize));
    file.write(reinterpret_cast<const char*>(eyefov.data()), eyeSize * sizeof(float));
    file.write(reinterpret_cast<const char*>(eyedir.data()), eyeSize * sizeof(float));
    
    // Save brain
    brain.saveToStream(file);
    
    // Save mutations
    size_t mutSize = mutations.size();
    file.write(reinterpret_cast<const char*>(&mutSize), sizeof(mutSize));
    for (const std::string& mutation : mutations) {
        size_t strSize = mutation.size();
        file.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        file.write(mutation.c_str(), strSize);
    }
}

void Agent::loadFromStream(std::ifstream& file)
{
    // Load basic properties
    file.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
    file.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
    file.read(reinterpret_cast<char*>(&health), sizeof(health));
    file.read(reinterpret_cast<char*>(&angle), sizeof(angle));
    file.read(reinterpret_cast<char*>(&red), sizeof(red));
    file.read(reinterpret_cast<char*>(&gre), sizeof(gre));
    file.read(reinterpret_cast<char*>(&blu), sizeof(blu));
    file.read(reinterpret_cast<char*>(&w1), sizeof(w1));
    file.read(reinterpret_cast<char*>(&w2), sizeof(w2));
    file.read(reinterpret_cast<char*>(&boost), sizeof(boost));
    file.read(reinterpret_cast<char*>(&spikeLength), sizeof(spikeLength));
    file.read(reinterpret_cast<char*>(&age), sizeof(age));
    file.read(reinterpret_cast<char*>(&spiked), sizeof(spiked));
    
    // Load input/output vectors
    size_t inSize, outSize;
    file.read(reinterpret_cast<char*>(&inSize), sizeof(inSize));
    file.read(reinterpret_cast<char*>(&outSize), sizeof(outSize));
    in.resize(inSize);
    out.resize(outSize);
    file.read(reinterpret_cast<char*>(in.data()), inSize * sizeof(float));
    file.read(reinterpret_cast<char*>(out.data()), outSize * sizeof(float));
    
    // Load other properties
    file.read(reinterpret_cast<char*>(&repcounter), sizeof(repcounter));
    file.read(reinterpret_cast<char*>(&gencount), sizeof(gencount));
    file.read(reinterpret_cast<char*>(&hybrid), sizeof(hybrid));
    file.read(reinterpret_cast<char*>(&clockf1), sizeof(clockf1));
    file.read(reinterpret_cast<char*>(&clockf2), sizeof(clockf2));
    file.read(reinterpret_cast<char*>(&soundmul), sizeof(soundmul));
    file.read(reinterpret_cast<char*>(&indicator), sizeof(indicator));
    file.read(reinterpret_cast<char*>(&ir), sizeof(ir));
    file.read(reinterpret_cast<char*>(&ig), sizeof(ig));
    file.read(reinterpret_cast<char*>(&ib), sizeof(ib));
    file.read(reinterpret_cast<char*>(&selectflag), sizeof(selectflag));
    file.read(reinterpret_cast<char*>(&dfood), sizeof(dfood));
    file.read(reinterpret_cast<char*>(&give), sizeof(give));
    file.read(reinterpret_cast<char*>(&id), sizeof(id));
    file.read(reinterpret_cast<char*>(&herbivore), sizeof(herbivore));
    file.read(reinterpret_cast<char*>(&MUTRATE1), sizeof(MUTRATE1));
    file.read(reinterpret_cast<char*>(&MUTRATE2), sizeof(MUTRATE2));
    file.read(reinterpret_cast<char*>(&temperature_preference), sizeof(temperature_preference));
    file.read(reinterpret_cast<char*>(&smellmod), sizeof(smellmod));
    file.read(reinterpret_cast<char*>(&soundmod), sizeof(soundmod));
    file.read(reinterpret_cast<char*>(&hearmod), sizeof(hearmod));
    file.read(reinterpret_cast<char*>(&eyesensmod), sizeof(eyesensmod));
    file.read(reinterpret_cast<char*>(&bloodmod), sizeof(bloodmod));
    
    // Load eye properties
    size_t eyeSize;
    file.read(reinterpret_cast<char*>(&eyeSize), sizeof(eyeSize));
    eyefov.resize(eyeSize);
    eyedir.resize(eyeSize);
    file.read(reinterpret_cast<char*>(eyefov.data()), eyeSize * sizeof(float));
    file.read(reinterpret_cast<char*>(eyedir.data()), eyeSize * sizeof(float));
    
    // Load brain
    brain.loadFromStream(file);
    
    // Load mutations
    size_t mutSize;
    file.read(reinterpret_cast<char*>(&mutSize), sizeof(mutSize));
    mutations.clear();
    mutations.reserve(mutSize);
    for (size_t i = 0; i < mutSize; ++i) {
        size_t strSize;
        file.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
        std::string mutation(strSize, '\0');
        file.read(&mutation[0], strSize);
        mutations.push_back(mutation);
    }
}
