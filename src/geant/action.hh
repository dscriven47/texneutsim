#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"
#include "run.hh"
#include "action.hh"
#include "stepping.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
  public:
    MyActionInitialization();
    ~MyActionInitialization();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};

#endif