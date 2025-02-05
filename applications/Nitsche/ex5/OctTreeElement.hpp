#ifndef __femus_OctTreeElement_hpp__
#define __femus_OctTreeElement_hpp__

class OctTreeElement {
    //specific data members
    std::vector<std::vector < double > >  _xv;
    std::vector<std::vector < double > >  _xi;
    std::vector< OctTreeElement >  _child;
    bool _haveChilderen = false;
    std::vector < std::vector < double> > _xg[2];
    std::vector < std::vector < double > > _phi[2];
    std::vector < double> _weight[2];
    bool _femQuantitiesAreBuilt[2];

    std::vector<std::vector < std::vector < std::pair < unsigned, double> > > > _fakePMatrix;

    //share data members
    static unsigned _counter;
    static unsigned _counterFEM[2];
    std::vector<std::vector < std::vector < std::pair < unsigned, double> > > > &_PMatrix  = _fakePMatrix;
    const elem_type *_fem[2];

  public:

    const std::vector < std::vector < double> > & GetNodeCoordinates()  {
      return _xv;
    }

    const unsigned GetNumberOfChildren() {
      return _PMatrix.size();
    }

    const std::vector < std::vector < double> > & GetGaussCoordinates(const unsigned &l) {
      if(!_femQuantitiesAreBuilt[l]) {
        BuildFemQuantities(l);
      }
      return _xg[l];
    }

    const std::vector < std::vector < double> > & GetGaussShapeFunctions(const unsigned &l) {
      if(!_femQuantitiesAreBuilt[l]) {
        BuildFemQuantities(l);
      }
      return _phi[l];
    }

    const std::vector < double> & GetGaussWeights(const unsigned &l) {
      if(!_femQuantitiesAreBuilt[l]) {
        BuildFemQuantities(l);
      }
      return _weight[l];
    }

    void Init(const std::vector<std::vector < double > >  &xv,
              const std::vector<std::vector < double > >  &xi,
              const std::vector<std::vector < std::vector < std::pair < unsigned, double> > > > &PMatrix,
              const elem_type *fem0, const elem_type *fem1, const unsigned &prelocateMemoryLevelMax) {

      this->Clear();
      _xv = xv;
      _xi = xi;
      _PMatrix = PMatrix;

      _counter = 1;
      _counterFEM[0] = 0;
      _counterFEM[1] = 0;

      _haveChilderen = false;
      _fem[0] = fem0;
      _fem[1] = fem1;
      _femQuantitiesAreBuilt[0] = false;
      _femQuantitiesAreBuilt[1] = false;
      if(prelocateMemoryLevelMax > 0) {
        std::vector < unsigned > ng(2);
        for(unsigned l = 0; l < 2; l++) {
          ng[l] = _fem[l]->GetGaussPointNumber();
        }

        unsigned numberOfChildren = _PMatrix.size();
        this->PrelocateMemory(0, prelocateMemoryLevelMax, ng, numberOfChildren);

      }
    }

  private:

    void PrelocateMemory(const unsigned &level, const unsigned &prelocateMemoryLevelMax,
                         const std::vector < unsigned > &ng, const unsigned &numberOfChildren) {

      for(unsigned l = 0; l < 2; l++) {
        _xg[l].resize(ng[l]);
        _phi[l].resize(ng[l]);
        _weight[l].resize(ng[l]);
        for(unsigned ig = 0; ig < ng[l]; ig++) {
          _xg[l][ig].resize(_xv.size());
          _phi[l][ig].resize(_xv[0].size());
        }
      }
      if(prelocateMemoryLevelMax > level + 1) {
        _child.resize(numberOfChildren);
        for(unsigned i = 0; i < numberOfChildren; i++) {
          _child[i]._xv.resize(_xv.size());
          _child[i]._xi.resize(_xi.size());
          for(unsigned j = 0; j < _xv.size(); j++) {
            _child[i]._xv[j].resize(_xv[j].size());
            _child[i]._xi[j].resize(_xi[j].size());
          }
          _child[i]._haveChilderen = false;
          _child[i]._femQuantitiesAreBuilt[0] = false;
          _child[i]._femQuantitiesAreBuilt[1] = false;
          _child[i].PrelocateMemory(level + 1, prelocateMemoryLevelMax, ng, numberOfChildren);
        }
      }
    }

    void BuildFemQuantities(const unsigned &l) {
      _counterFEM[l]++;
      unsigned dim = _xv.size();
      unsigned numberOfNodes = _xv[0].size();

      std::vector < double>  xiFg(dim); //local coordinates of the Father element evaluated in Child element
      const double *phiC; //Shape functions of the Child element

      unsigned ng = _fem[l]->GetGaussPointNumber();
      _xg[l].resize(ng);
      _phi[l].resize(ng);
      _weight[l].resize(ng);

      for(unsigned ig = 0; ig < ng; ig++) {
        _fem[l]->GetGaussQuantities(_xv, ig, _weight[l][ig], phiC);
        _xg[l][ig].assign(dim, 0.);
        xiFg.assign(dim, 0.);
        for(unsigned k = 0; k < dim; k++) {
          for(unsigned j = 0; j < numberOfNodes; j++) {
            _xg[l][ig][k] += _xv[k][j] * phiC[j];
            xiFg[k] += _xi[k][j] * phiC[j];
          }
        }
        _phi[l][ig].resize(numberOfNodes);
        _fem[l]->GetPhi(_phi[l][ig], xiFg);
      }
      _femQuantitiesAreBuilt[l]  = true;
    }

  public:
    void Clear() {
      if(_haveChilderen) {
        for(unsigned i = 0; i < _child.size(); i++) {
          _child[i].Clear();
        }
      }
      _haveChilderen = false;
      for(unsigned l = 0; l < 2; l++) {
        _femQuantitiesAreBuilt[l] = false;
      }
    }

    OctTreeElement *GetElement(const std::vector<unsigned> &genealogy) {
      return this->GetElement(genealogy, 0);
    }

  private:
    OctTreeElement *GetElement(const std::vector<unsigned> &genealogy, unsigned level) {
      if(level == genealogy.size()) {
        return this;
      }
      else {
        if(this->_haveChilderen == false) {
          this->GenerateChildren();
        }
        return this->_child[ genealogy[level] ].GetElement(genealogy, level + 1);
      }
    }

    void GenerateChildren() {

      _counter += _PMatrix.size();
      _child.resize(_PMatrix.size());

      for(unsigned i = 0; i < _child.size(); i++) {
        _child[i]._xv.resize(_xv.size());
        _child[i]._xi.resize(_xi.size());
        for(unsigned j = 0; j < _xv.size(); j++) {
          _child[i]._xv[j].resize(_xv[j].size());
          _child[i]._xi[j].resize(_xi[j].size());
        }
        _child[i]._PMatrix = _PMatrix;
        _child[i]._haveChilderen = false;
        _child[i]._fem[0] = _fem[0];
        _child[i]._fem[1] = _fem[1];
      }

      std::vector< OctTreeElement >::iterator xCi;
      std::vector < std::vector<double>>::iterator xCik;
      std::vector<double>::iterator xCikj;

      std::vector < std::vector<double>>::const_iterator xFk;

      std::vector <std::vector <std::vector < std::pair<unsigned, double>>>>::const_iterator Pi;
      std::vector <std::vector < std::pair<unsigned, double>>>::const_iterator Pij;
      std::vector < std::pair<unsigned, double>>::const_iterator Pijl;

      for(Pi = _PMatrix.begin(), xCi = _child.begin(); Pi != _PMatrix.end(); xCi++, Pi++) {
        for(xCik = (*xCi)._xv.begin(), xFk = _xv.begin(); xCik != (*xCi)._xv.end(); xCik++, xFk++) {
          for(Pij = (*Pi).begin(), xCikj = (*xCik).begin(); Pij != (*Pi).end(); Pij++, xCikj++) {
            *xCikj = 0.;
            for(Pijl = (*Pij).begin(); Pijl != (*Pij).end(); Pijl++) {
              *xCikj += Pijl->second * (*xFk)[Pijl->first];
            }
          }
        }
      }

      for(Pi = _PMatrix.begin(), xCi = _child.begin(); Pi != _PMatrix.end(); xCi++, Pi++) {
        for(xCik = (*xCi)._xi.begin(), xFk = _xi.begin(); xCik != (*xCi)._xi.end(); xCik++, xFk++) {
          for(Pij = (*Pi).begin(), xCikj = (*xCik).begin(); Pij != (*Pi).end(); Pij++, xCikj++) {
            *xCikj = 0.;
            for(Pijl = (*Pij).begin(); Pijl != (*Pij).end(); Pijl++) {
              *xCikj += Pijl->second * (*xFk)[Pijl->first];
            }
          }
        }
      }
      _haveChilderen = true;
    }

    void PrintElement(std::ofstream & fout) const {
      for(unsigned j = 0; j < 4; j++) {
        fout << _xv[0][j] << " " << _xv[1][j] << std::endl;
      }
      fout << _xv[0][0] << " " << _xv[1][0] << std::endl << std::endl;
      if(_haveChilderen) {
        for(unsigned i = 0; i < _child.size(); i++) {
          _child[i].PrintElement(fout);
        }
      }
    }

  public:
    void PrintElement(std::string filename, bool clearFile = true) const {
      std::ofstream fout;
      if(clearFile) {
        fout.open(filename);
      }
      else {
        fout.open(filename, std::ios::app);
      }
      PrintElement(fout);
      fout.close();
    }

    void PrintCounter() const {
      std::cout << "The total number of elements formed is " << _counter << std::endl;
      std::cout << "The total number of FEM0 formed is " << _counterFEM[0] << std::endl;
      std::cout << "The total number of FEM1 formed is " << _counterFEM[1] << std::endl;
    }

};

unsigned OctTreeElement::_counter = 0;
unsigned OctTreeElement::_counterFEM[2] = {0, 0};

#endif
