
namespace plb{
  
  template<typename T, template<typename U> class Descriptor>
  ProbeManager<T,Descriptor>::ProbeManager(std::string const &velocityFname_, 
                                           std::string const &densityFname_, 
                                           ArrayVector const &velocityProbeLocations_,
                                           ArrayVector const &densityProbeLocations_,
                                           IncomprFlowParam<T> const &param_)
    : velFname(velocityFname_), rhoFname(densityFname_),
      velProbes(velocityProbeLocations_),
      rhoProbes(densityProbeLocations_),
    parameters(param_){ }
  
  template<typename T, template<typename U> class Descriptor>
  void ProbeManager<T,Descriptor>::writeHeaders() 
  {
    if(velProbes.size() > 0){
      plb_ofstream s(velFname.c_str());
      writeProbeCoords(s,velProbes);
      s.close();
    }
    if(rhoProbes.size() > 0){
      plb_ofstream s(rhoFname.c_str());
      writeProbeCoords(s,rhoProbes);
      s.close();
    }
  }

  template<typename T, template<typename U> class Descriptor>
  void ProbeManager<T,Descriptor>::writeVelocityProbes(MultiBlockLattice3D<T,Descriptor> &lattice, plint const iT)
  {
    T const u_max = parameters.getLatticeU();

    VelocitySingleProbe3D<T,Descriptor> p(velProbes);
    applyProcessingFunctional(p,lattice.getBoundingBox(),lattice);
    ArrayVector v = p.getVelocities();

    plb_ofstream s(velFname.c_str(),std::fstream::out | std::fstream::app);
    
    s << std::setfill(' ') << std::setw(linewidth-1) << iT*parameters.getDeltaT();
    for(pluint i=0;i<v.size();i++){
      s << std::setfill(' ') << std::setw(linewidth) << '('
        << std::setprecision(precision) << v[i][0]/u_max << " " 
        << std::setprecision(precision) << v[i][1]/u_max << " "
        << std::setprecision(precision) << v[i][2]/u_max << ")";
    }
    s << std::endl;

    s.close();
  }

  template<typename T, template<typename U> class Descriptor>
  void ProbeManager<T,Descriptor>::writeDensityProbes(MultiBlockLattice3D<T,Descriptor> &lattice, plint const iT)
  {
    T const rho_fact = 1;

    DensitySingleProbe3D<T,Descriptor> probe(rhoProbes);
    applyProcessingFunctional(probe,lattice.getBoundingBox(),lattice);
    ScalarVector p = probe.getDensities();

    plb_ofstream s(rhoFname.c_str(),std::fstream::out | std::fstream::app);
    
    s << std::setfill(' ') << std::setw(linewidth-1) << iT*parameters.getDeltaT();
    for(pluint i=0;i<p.size();i++){
      s << std::setfill(' ') << std::setw(linewidth) 
        << std::setprecision(precision) << (p[i]-1.)*rho_fact;
    }
    s << std::endl;

    s.close();
  }


  template<typename T, template<typename U> class Descriptor>
  void ProbeManager<T,Descriptor>::writeProbeCoords(plb_ofstream &s, ArrayVector &probes)
  {
    T const nx = parameters.getResolution();
    char coordStr[3] = {'x','y','z'};
    for(pluint i=0;i<3;i++){
      s << "#" << std::setfill(' ') << std::setw(linewidth-2) << coordStr[i];
      for(pluint iPr=0;iPr<velProbes.size();iPr++)
        s << std::setfill(' ') << std::setw(linewidth) << velProbes[iPr][i]/nx;
      s << std::endl;
    }
    s << "#" << std::setfill(' ') << std::setw(linewidth-2) << "Time" << std::endl;
  }
  
}; // namespace plb
