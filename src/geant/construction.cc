#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
  fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

  //fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
  //fMessenger->DeclareProperty("nRows", nRows, "Number of rows");

  // set the low and high limits for secondary particle creation
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(0.1 * eV, 100 * GeV);

  DefineMaterials();

  // size of the world volume
  xWorld = .75*m;
  yWorld = .75*m;
  zWorld = .75*m;
}

MyDetectorConstruction::~MyDetectorConstruction()
{

}

void MyDetectorConstruction::DefineMaterials()
{
  G4NistManager *nist = G4NistManager::Instance(); // this will call a nist database which can be used to source materials

  // make our first material, fused silica
  SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
  SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
  SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

  // another definition for water
  H2O = new G4Material("H2O", 1.000*g/cm3, 2);
  H2O->AddElement(nist->FindOrBuildElement("H"), 2);
  H2O->AddElement(nist->FindOrBuildElement("O"), 1);

  // definition for carbon
  C = nist->FindOrBuildElement("C");

  // create a compount material made of elements and compounds
  Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
  Aerogel->AddMaterial(SiO2, 62.5*perCent);
  Aerogel->AddMaterial(H2O, 37.4*perCent);
  Aerogel->AddElement(C, 0.1*perCent);

  worldMat = nist->FindOrBuildMaterial("G4_AIR");

  // setup the refractive properties of the aerogel
  // 1.239841939 is the conversion from nm to eV
  G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
  G4double rindexAerogel[2] = {1.1,1.1};
  G4double rindexWorld[2] = {1.,1.};
  G4double rindexNaI[2] = {1.78,1.78};
  G4double reflectivity[2] = {1.0,1.0};

  G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable(); // make a material property for the aerogel and add the above properties to the table
  mptAerogel->AddProperty("RINDEX",energy,rindexAerogel,2);
  Aerogel->SetMaterialPropertiesTable(mptAerogel); // now add the material properties table to the material
  
  G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
  mptWorld->AddProperty("RINDEX",energy,rindexWorld,2);
  worldMat->SetMaterialPropertiesTable(mptWorld);

  Na = nist->FindOrBuildElement("Na");
  I = nist->FindOrBuildElement("I");
  NaI = new G4Material("NaI",3.67*g/cm3,2);
  NaI->AddElement(Na,1);
  NaI->AddElement(I,1);

  G4double fraction[2] = {1.0,1.0}; // fast component spectrum

  // optical properties for NaI scintillator (simple)
  G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
  mptNaI->AddProperty("RINDEX",energy,rindexNaI,2);
  mptNaI->AddProperty("FASTCOMPONENT",energy,fraction,2,true);
  mptNaI->AddConstProperty("SCINTILLATIONYIELD",38000./MeV);
  mptNaI->AddConstProperty("RESOLUTIONSCALE",1.0);
  mptNaI->AddConstProperty("FASTTIMECONSTANT",250*ns,true);
  mptNaI->AddConstProperty("YIELDRATIO",1.,true);
  NaI->SetMaterialPropertiesTable(mptNaI);

  // optical properties for the optical coating of NaI
  mirrorSurface = new G4OpticalSurface("mirrorSurface");
  mirrorSurface->SetType(dielectric_metal);
  mirrorSurface->SetFinish(ground);
  mirrorSurface->SetModel(unified);
  G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
  mptMirror->AddProperty("REFLECTIVITY",energy,reflectivity,2);
  mirrorSurface->SetMaterialPropertiesTable(mptMirror);


  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;

  //***Elements
  H = nist->FindOrBuildElement("H");
  C = nist->FindOrBuildElement("C");
  N = nist->FindOrBuildElement("N");
  O = nist->FindOrBuildElement("O");

  //***Materials
  //p-Terphenyl
  pTerp = new G4Material("pTerp",1.23*g/cm3,2);
  pTerp->AddElement(H,14);
  pTerp->AddElement(C,18);

  //EJ-560 Silicone Optical Pad
  OpticalPadSilicone = new G4Material("OpticalPad_Silicone",density=1.03*g/cm3,2);
  OpticalPadSilicone->AddElement(H,6);
  OpticalPadSilicone->AddElement(C,2);



  //Aluminum
  Al = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);
  //Vacuum

  Vacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
                          density=universe_mean_density,kStateGas,0.1*kelvin,
                          1.e-19*pascal);

  //Air
  Air = new G4Material("Air", density= 1.29*mg/cm3, 2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  //Glass
  Glass = new G4Material("Glass", density=1.032*g/cm3,2);
  Glass->AddElement(C,91.533*perCent);
  Glass->AddElement(H,8.467*perCent);
 
  //***Material properties tables
  G4double pterp_Energy[]    = { 7.0*eV , 7.07*eV, 7.14*eV };
  G4int pterpnum = sizeof(pterp_Energy)/sizeof(G4double);
  G4double pterp_SCINT[] = { 0.1, 1.0, 0.1 };
  assert(sizeof(pterp_SCINT) == sizeof(pterp_Energy));
  G4double pterp_RIND[]  = { 1.65 , 1.65, 1.65 };
  assert(sizeof(pterp_RIND) == sizeof(pterp_Energy));
  G4double pterp_ABSL[]  = { 10.*cm, 10.*cm, 10.*cm};
  assert(sizeof(pterp_ABSL) == sizeof(pterp_Energy));
  G4MaterialPropertiesTable *pTerp_mt = new G4MaterialPropertiesTable();
  pTerp_mt->AddProperty("FASTCOMPONENT", pterp_Energy, pterp_SCINT, pterpnum, true);
  pTerp_mt->AddProperty("SLOWCOMPONENT", pterp_Energy, pterp_SCINT, pterpnum, true);
  pTerp_mt->AddProperty("RINDEX",        pterp_Energy, pterp_RIND,  pterpnum);
  pTerp_mt->AddProperty("ABSLENGTH",     pterp_Energy, pterp_ABSL,  pterpnum);
  pTerp_mt->AddConstProperty("SCINTILLATIONYIELD",0./MeV);
  pTerp_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  pTerp_mt->AddConstProperty("FASTTIMECONSTANT",3.3*ns, true);
  pTerp_mt->AddConstProperty("SLOWTIMECONSTANT",31.*ns, true);
  pTerp_mt->AddConstProperty("YIELDRATIO",1.0, true);
  pTerp->SetMaterialPropertiesTable(pTerp_mt);
  // Set the Birks Constant for the LXe scintillator
  //pTerp->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
 


  // glass settings
  G4double glass_Energy[] = { 7.0*eV , 7.07*eV, 7.14*eV };
  G4int glassnum = sizeof(glass_Energy)/sizeof(G4double);;
  G4double glass_RIND[]={1.49,1.49,1.49};
  assert(sizeof(glass_RIND) == sizeof(glass_Energy));
  G4double glass_AbsLength[]={420.*cm,420.*cm,420.*cm};
  assert(sizeof(glass_AbsLength) == sizeof(glass_Energy));
  G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH",glass_Energy,glass_AbsLength,glassnum);
  glass_mt->AddProperty("RINDEX",glass_Energy,glass_RIND,glassnum);
  Glass->SetMaterialPropertiesTable(glass_mt);

  // silicone pad settings
  G4double pad_Energy[] = { 7.0*eV , 7.07*eV, 7.14*eV };
  G4int padnum = sizeof(pad_Energy)/sizeof(G4double);
  G4double opticalpadsilicone_RIND[]={1.49,1.49,1.49};
  assert(sizeof(opticalpadsilicone_RIND) == sizeof(pad_Energy));
  G4double opticalpadsilicone_AbsLength[]={420.*cm,420.*cm,420.*cm};
  assert(sizeof(opticalpadsilicone_AbsLength) == sizeof(pad_Energy));
  G4MaterialPropertiesTable *opticalpadsilicone_mt = new G4MaterialPropertiesTable();
  opticalpadsilicone_mt->AddProperty("ABSLENGTH",pad_Energy,opticalpadsilicone_AbsLength,padnum);
  opticalpadsilicone_mt->AddProperty("RINDEX",pad_Energy,opticalpadsilicone_RIND,padnum);
  OpticalPadSilicone->SetMaterialPropertiesTable(opticalpadsilicone_mt);

  G4double vacuum_Energy[]={2.0*eV,7.0*eV,7.14*eV};
  const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
  G4double vacuum_RIND[]={1.,1.,1.};
  assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));
  G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);
  Vacuum->SetMaterialPropertiesTable(vacuum_mt);
  Air->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex

}





void MyDetectorConstruction::ConstructScintillator()
{
  fCube_mult = 6; // number of crystals per bar

  imax = 16; // 16 Rows
  jmax = 8;  //  8 Columns 

  G4double xspacing=2.86258*cm;
  G4double zspacing=4.13766*cm;

  G4double xoffset = (imax-1)*xspacing/2.;
  G4double yoffset = 0.;
  G4double zoffset = -0.025*m;

  G4double fD_mtl = 0.0635*cm;

  G4double fCube_x = 2.0*cm;
  G4double fCube_y = 2.0*cm;
  G4double fCube_z = 2.0*cm;

  G4double fPad_x = 0.1*cm;
  G4double fPad_y = fCube_y;
  G4double fPad_z = fCube_z;

  G4double fCube_mult = 6;

  G4double fScint_x = fCube_x*fCube_mult + (fCube_mult+1)*fPad_x;
  G4double fScint_y = fCube_y;
  G4double fScint_z = fCube_z;


  G4double housing_x = fScint_x-fPad_x*2;
  G4double housing_y = fScint_y+2.*fD_mtl;
  G4double housing_z = fScint_z+2.*fD_mtl;

  checkGeometry = true;

  ///////////////////////////// Housing
  G4Box* outerBox = new G4Box("Outer Box",housing_x/2.,housing_y/2.,housing_z/2.);
  G4Box* innerBox = new G4Box("Inner Box",housing_x/2.,fScint_y/2.,fScint_z/2.);
  fHousing_box = new G4SubtractionSolid("housing_box",outerBox,innerBox);
  fHousing_log = new G4LogicalVolume(fHousing_box,G4Material::GetMaterial("Al"),"housing_log",0,0,0);
  G4RotationMatrix* housing_rot = new G4RotationMatrix();
  housing_rot->rotateX(0*deg);
  housing_rot->rotateY(0*deg);
  housing_rot->rotateZ(90*deg);	
  G4ThreeVector housing_loc = G4ThreeVector(0.,0.,0.);
    
  //Housing_Multiplier
  //i=Rows, j=Columns
  for(G4int i=0; i<imax; i++){ 
    for(G4int j=0; j<jmax; j++){
      housing_loc.setX(i*xspacing-xoffset);
      housing_loc.setZ(j*zspacing-zoffset);
      new G4PVPlacement(housing_rot,housing_loc,fHousing_log,"phys_Housing", logicWorld,false,i+j*(jmax-1),checkGeometry);	
      
    }
  }

  
  // define the housing skin
  G4OpticalSurface *housingSkin = new G4OpticalSurface("housingSkin");
  G4LogicalSkinSurface *housingSurface = new G4LogicalSkinSurface("housingSkin",fHousing_log,housingSkin);
  housingSkin->SetType(dielectric_metal);
  housingSkin->SetFinish(polishedvm2000air);
  housingSkin->SetModel(glisur);

  G4double pp[] = {1.0*eV, 10.*eV};
  G4int num = sizeof(pp)/sizeof(G4double);
  G4double reflectivity[] = {1., 1.};
  assert(sizeof(reflectivity) == sizeof(pp));
  G4double efficiency[] = {0.0, 0.0};
  assert(sizeof(efficiency) == sizeof(pp));
  
  G4MaterialPropertiesTable* housingSkinProperty = new G4MaterialPropertiesTable();
  housingSkinProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
  housingSkinProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
  housingSkin->SetMaterialPropertiesTable(housingSkinProperty);

 
  // create the optical pads
  fPad_box = new G4Box("pad_box",fPad_x/2,fPad_y/2,fPad_z/2);
  fPad_log = new G4LogicalVolume(fPad_box,G4Material::GetMaterial("OpticalPad_Silicone"),"pad_log",0,0,0);

  for(G4int i=0; i<imax; i++){ 
    for(G4int j=0; j<jmax; j++){
      for(int k=0; k<fCube_mult+1; k++) {
        G4RotationMatrix* Rotation = new G4RotationMatrix();
        Rotation->rotateX(0*deg);
        Rotation->rotateY(0*deg);
        Rotation->rotateZ(90*deg);	
   
        G4ThreeVector pad_loc = G4ThreeVector(i*xspacing-xoffset,(-fScint_x+fPad_x)/2+(fCube_x+fPad_x)*k,j*zspacing-zoffset);
        pad = new G4PVPlacement(Rotation,pad_loc,fPad_log,
                                "pad_phys", logicWorld,
                                false,k+j*(fCube_mult+1)+i*(jmax*(fCube_mult+1)),checkGeometry);

        //Surface properties for the optical pad
        G4OpticalSurface* padSkin = new G4OpticalSurface("PadSkin");

        G4LogicalSkinSurface* padSurface = new G4LogicalSkinSurface("PadSkin",fPad_log,padSkin);

        padSkin->SetType(dielectric_dielectric);
        padSkin->SetFinish(polished);
        padSkin->SetModel(glisur);

        G4double pp[] = {2.0*eV, 3.5*eV};
        G4int num = sizeof(pp)/sizeof(G4double);
        G4double reflectivity[] = {1., 1.};
        assert(sizeof(reflectivity) == sizeof(pp));
        G4double efficiency[] = {0.0, 0.0};
        assert(sizeof(efficiency) == sizeof(pp));
        
        G4MaterialPropertiesTable *scintWrapProperty = new G4MaterialPropertiesTable();
        scintWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
        scintWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
        padSkin->SetMaterialPropertiesTable(scintWrapProperty);
      }
    }
  }

  

  // Create the discrete scintillator cubes
  fScint_box = new G4Box("scint_box",fCube_x/2.,fScint_y/2.,fScint_z/2.); 
  fScint_log = new G4LogicalVolume(fScint_box,pTerp,"scint_log",0,0,0);

  // Set user limits for the detector
  G4UserLimits* userLimits = new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, 0.01 * mm);
  fScint_log->SetUserLimits(userLimits);

  G4int scintCopyNo = 0;

  for(G4int i=0; i<imax; i++){ 
    for(G4int j=0; j<jmax; j++){
      for(int k=0; k<fCube_mult; k++){
      
        scintCopyNo = k+j*(fCube_mult+1)+i*(jmax*(fCube_mult+1));

        G4ThreeVector cube_loc = G4ThreeVector(i*xspacing-xoffset,-fScint_x/2+fCube_x/2+fPad_x+(fCube_x+fPad_x)*k,j*zspacing-zoffset);
        cube_phys = new G4PVPlacement(0,cube_loc,fScint_log,"scintillator",logicWorld,false,scintCopyNo,checkGeometry);
        
        copyPositions[scintCopyNo] = cube_loc;
        
        //G4cout << k+j*(fCube_mult+1)+i*(jmax*(fCube_mult+1)) << G4endl;

        //Surface properties for the scintillator cubes
        G4OpticalSurface* cubeSkin = new G4OpticalSurface("cubeSkin");

        //G4LogicalSkinSurface* abc = new G4LogicalSkinSurface("cubeSkin",fScint_log,cubeSkin);
        G4LogicalBorderSurface *cubeSurface = new G4LogicalBorderSurface("cubeSkin",cube_phys,physWorld,cubeSkin);

        cubeSkin->SetType(dielectric_dielectric);
        cubeSkin->SetFinish(groundair);
        cubeSkin->SetModel(glisur);

        G4double pp[] = {2.0*eV, 3.5*eV};
        G4int num = sizeof(pp)/sizeof(G4double);
        G4double reflectivity[] = {1., 1.};
        assert(sizeof(reflectivity) == sizeof(pp));
        G4double efficiency[] = {0.0, 0.0};
        assert(sizeof(efficiency) == sizeof(pp));
        
        G4MaterialPropertiesTable* cubeSkinProperty = new G4MaterialPropertiesTable();

        cubeSkinProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
        cubeSkinProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
        cubeSkin->SetMaterialPropertiesTable(cubeSkinProperty);
      }
    }
  } 


  ////////////////////Build PMTs
  G4double innerRadius_pmt = 0.*cm;
  G4double fOuterRadius_pmt = 17.5/2*mm;
  G4double height_pmt = 88./2*mm; //30*fD_mtl/2.;
  G4double startAngle_pmt = 0.*deg;
  G4double spanningAngle_pmt = 360.*deg;
  G4double cathode_depth = 0.4*cm;
  G4double cathode_thickness = 0.02*cm;
 
  //the "photocathode" is a metal slab at the back of the glass that
  //is only a very rough approximation of the real thing since it only
  //absorbs or detects the photons based on the efficiency set below
  fPmt = new G4Tubs("pmt_tube",innerRadius_pmt,fOuterRadius_pmt,
                    height_pmt,startAngle_pmt,spanningAngle_pmt);

  fPhotocath = new G4Tubs("photocath_tube",innerRadius_pmt,fOuterRadius_pmt,
                          cathode_thickness,startAngle_pmt,spanningAngle_pmt);

  fPmt_log = new G4LogicalVolume(fPmt,G4Material::GetMaterial("Glass"),
                                "pmt_log");
  fPhotocath_log = new G4LogicalVolume(fPhotocath,
                                      G4Material::GetMaterial("Al"),
                                      "photocath_log");
  //G4RotationMatrix* Rotation = new G4RotationMatrix();
  //Rotation->rotateX(0*deg);
  //Rotation->rotateY(0*deg);
  //Rotation->rotateZ(90*deg);	
  
  G4ThreeVector photocath_loc = G4ThreeVector(0.,0.,height_pmt-cathode_depth);
  
  new G4PVPlacement(0,photocath_loc,fPhotocath_log,"photocath",fPmt_log,false,0,checkGeometry);	
  
  // define the photocathode skin
  G4double ephoton[] = {7.0*eV, 7.14*eV};
  num = sizeof(ephoton)/sizeof(G4double);
  G4double photocath_EFF[]={1.,1.}; //Enables 'detection' of photons
  assert(sizeof(photocath_EFF) == sizeof(ephoton));
  G4double photocath_ReR[]={1.92,1.92};
  assert(sizeof(photocath_ReR) == sizeof(ephoton));
  G4double photocath_ImR[]={1.69,1.69};
  assert(sizeof(photocath_ImR) == sizeof(ephoton));
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  G4OpticalSurface* photocath_opsurf= new G4OpticalSurface("photocath_opsurf",glisur,polished,dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);
  new G4LogicalSkinSurface("photocath_surf",fPhotocath_log,photocath_opsurf);
  

  // make the mu-metal shields (in the simulation it's the grey tube)
  G4double fmushield_z = 9.4*cm;
  G4double fmushield_rmax = 2./2*cm;
  G4double fmushield_rmin = 1.892/2*cm;

  ///////////////Mu_Metal Shield Multiplier ////////////
  for(G4int i=0; i<imax; i++){ 
    for(G4int j=0; j<jmax; j++){ 
      for(int k=0; k<2; k++){
        fMuShield_tub = new G4Tubs("MuShield",fmushield_rmin,fmushield_rmax,fmushield_z/2.,0.*deg,360.*deg);
        int sign=0;
        if(k==0) sign=1;
        else sign=-1;
      
        G4ThreeVector fMuMetal_loc = G4ThreeVector(i*xspacing-xoffset,
                                                    -sign*fScint_x+sign*fCube_x-sign*fPad_x*(fCube_mult/2+0.5),
                                                    j*zspacing-zoffset);

        G4RotationMatrix* mu_rotation = new G4RotationMatrix();
        mu_rotation->rotateX(90*deg);
        mu_rotation->rotateY(0*deg);
        mu_rotation->rotateZ(0*deg);
        fMuShield_log = new G4LogicalVolume(fMuShield_tub,G4Material::GetMaterial("Al"),"fMuShield_log",0,0,0);

        fmuMetal_phys = new G4PVPlacement(mu_rotation,fMuMetal_loc,fMuShield_log,"MuShield",
                                logicWorld,false,k+j*(fCube_mult+1)+i*(jmax*(fCube_mult+1)),checkGeometry);


        // define the mumetal skin
        G4OpticalSurface* muSkin = new G4OpticalSurface("muSkin");

        G4LogicalSkinSurface *muSurface = new G4LogicalSkinSurface("muSkin",fMuShield_log,muSkin);
        muSkin->SetType(dielectric_metal);
        muSkin->SetFinish(polished);
        muSkin->SetModel(glisur);

        G4double pp[] = {1.0*eV, 10.*eV};
        num = sizeof(pp)/sizeof(G4double);
        G4double reflectivity[] = {1., 1.};
        assert(sizeof(reflectivity) == sizeof(pp));
        G4double efficiency[] = {0.0, 0.0};
        assert(sizeof(efficiency) == sizeof(pp));

        G4MaterialPropertiesTable* muSkinProperty 
        = new G4MaterialPropertiesTable();

        muSkinProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
        muSkinProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
        muSkin->SetMaterialPropertiesTable(muSkinProperty);

      }
    }
  }

  ///////////////Arrange pmts around the outside of housing/////////////

  
  /////////////Left PMT/////////////
  G4RotationMatrix* pmt_rm1 = new G4RotationMatrix();
  pmt_rm1->rotateX(90*deg);
  G4RotationMatrix* pmt_rm2 = new G4RotationMatrix();
  pmt_rm2->rotateX(-90*deg);
  G4ThreeVector fPmt_loc1;
  G4ThreeVector fPmt_loc2;
  
   
  ///////////////PMT illustration and Multiplier/////////////
  for(G4int i=0; i<imax; i++){ 
    for(G4int j=0; j<jmax; j++){ 
      
      fPmt_loc1 = G4ThreeVector(i*xspacing-xoffset,-fScint_x/2.-height_pmt,j*zspacing-zoffset);
      fPmt_loc2 = G4ThreeVector(i*xspacing-xoffset, fScint_x/2.+height_pmt,j*zspacing-zoffset); 
      
      new G4PVPlacement(pmt_rm1,fPmt_loc1,fPmt_log,"pmt",logicWorld,
                            false,0+j*(fCube_mult+1)+i*(jmax*(fCube_mult+1)),checkGeometry);

      new G4PVPlacement(pmt_rm2,fPmt_loc2,fPmt_log,"pmt",logicWorld,
                            false,1+j*(fCube_mult+1)+i*(jmax*(fCube_mult+1)),checkGeometry);
              
        //fPmtPositions.push_back(fPmt_loc);	
      
    }
  }

  //solidScintillator = new G4Tubs("solidScintillator",10*cm,20*cm,30*cm,0*deg,360*deg);
  //logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicalScintillator");
  //physScintillator = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicScintillator,"physScintillator",logicWorld, false, 0, true);
  //fScoringVolume = logicScintillator;
  fScoringVolume = fScint_log;
  
  VisAttributes();



}


void MyDetectorConstruction::ConstructTOF()
{
  solidDetector = new G4Box("solidDetector",1.*m,1.*m,0.1*m);
  logicDetector = new G4LogicalVolume(solidDetector, worldMat,"logicDetector");
  physDetector = new G4PVPlacement(0,G4ThreeVector(0.*m,0.*m,-4.*m),logicDetector,"physDetector",logicWorld,false,0,true);
  physDetector = new G4PVPlacement(0,G4ThreeVector(0.*m,0.*m, 3.*m),logicDetector,"physDetector",logicWorld,false,1,true);
}


G4VPhysicalVolume *MyDetectorConstruction::Construct()
{


  // Every material in geant4 has 3 parts
  //  The "solid", defines the size
  //  The "logical volume" which includes the material
  //  The "physical volume" places the volume in the geant4 simulation with coordinates and rotations, etc, and interacts with particles
  // The physical volume inherits the shape and logical volume from "logicWorld"
  solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld); // creates a volume with half length x=0.5,y=0.5,z=0.5, giving a volumes 1x1x1 m^3
  logicWorld = new G4LogicalVolume(solidWorld,worldMat,"locigWorld");
  physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);

  ConstructScintillator();


  return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{

  auto sdManager = G4SDManager::GetSDMpointer();
  // here we make a sensative detector and we tell the logical detector that it is this sensitive detector
  ScintillatorSD *scintDet = new ScintillatorSD("SensitiveScintillator");
  sdManager->AddNewDetector(scintDet);
  fScint_log->SetSensitiveDetector(scintDet);
  scintDet->SetCopyPositions(copyPositions);
  
}


void MyDetectorConstruction::VisAttributes()
{
  G4VisAttributes* housing_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8,1.0));
  housing_va->SetForceSolid(false);
  fHousing_log->SetVisAttributes(housing_va);

  G4VisAttributes* scint_va = new G4VisAttributes(G4Colour(0.4,0.4,0.8,0.4));
  scint_va->SetForceSolid(true);
  fScint_log->SetVisAttributes(scint_va);

  G4VisAttributes* cathode_va = new G4VisAttributes(G4Colour(0.8,0.4,0.4));
  cathode_va->SetForceSolid(false);
  fPhotocath_log->SetVisAttributes(cathode_va);

  G4VisAttributes* pmt_va = new G4VisAttributes(G4Colour(0.6,1.0,0.6,0.3));
  pmt_va->SetForceSolid(false);
  fPmt_log->SetVisAttributes(pmt_va);

  //G4VisAttributes* mushield_va = new G4VisAttributes(G4Colour(0.6,0.6,.6,0.3));
  //mushield_va->SetForceSolid(true);
  //fMuShield_log->SetVisAttributes(mushield_va);

  G4VisAttributes* pad_va = new G4VisAttributes(G4Colour(0.4,1.,0.4,0.4));
  pad_va->SetForceSolid(false);
  fPad_log->SetVisAttributes(pad_va);
}

