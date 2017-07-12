#include"DParticle.h"

// ----------------------------------------------------------------------------
// initialize
// ----------------------------------------------------------------------------
void DParticle::randomInit() {
  // get a random and valid position
  do {
    for (int i = 0; i < dimension; i++)
      position[i] = (int) (((double) (rand() % 1000) / 1000.0) * range[i]);
  } while (!isPositionValid());

  // velocity
  for (int i = 0; i < PosMax; i++) {
    PosVal pv;
    pv.posI = (int) (((double) (rand() % 1000) / 1000.0) * PosMax);            // random posI
    pv.valI = (int) (((double) (rand() % 1000) / 1000.0) * ValMax[pv.posI]);   // random valI

    // insert
    auto iter = velocity.find(pv);
    if (iter != velocity.end()) {
      iter->second += (double) (rand() % 1000) / 1000.0;
      if (iter->second > 1.0)
        iter->second = 1.0;
    } else {
      //velocity.insert( pair<PosVal,double>(pv, 1 - 0.5 * (double)(rand()%1000)/1000.0) );
      velocity.insert(pair<PosVal, double>(pv, (double) (rand() % 1000) / 1000.0));
    }
  }

  //for( iter = velocity.begin() ; iter != velocity.end() ; iter++ )
  //	cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;

  // set pbest as itself
  for (int i = 0; i < dimension; i++)
    pbest[i] = position[i];

  fitness_pbest = 0;
  fitness = 0;
}

// ----------------------------------------------------------------------------
// update pbest based on current position
// ----------------------------------------------------------------------------
void DParticle::setPbest(int fit) {
  if (isPositionValid()) {
    for (int i = 0; i < dimension; i++)
      pbest[i] = position[i];
    fitness_pbest = fit;
  }
}

// ----------------------------------------------------------------------------
// update velocity
// the new velocity is in TempV
// ----------------------------------------------------------------------------
void DParticle::velocityUpdate(double weight, double factor1, double factor2,
                               const int *gbest, double pro1) {
  // weight * velocity
  for (auto &x : velocity)
    x.second = x.second * weight;
  /*
  cout<<"term1"<<endl;
  for( map<PosVal,double>::iterator iter = velocity.begin() ; iter != velocity.end() ; iter++ )
    cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;
  cout<<"-------------------"<<endl;
  */

  // construct schemas of pbest and gbest
  // for each parameter combination
  vector<int *> allP = allPos(dimension, strength);
  int *val = new int[strength];
  int index = 0;
  for (auto pos : allP) {
    // determine whether each schema is in pbest or gbest
    int p_count = 0, g_count = 0;
    for (int c = 0; c < strength; c++) {
      if (pbest[pos[c]] == position[pos[c]])
        p_count++;
      if (gbest[pos[c]] == position[pos[c]])
        g_count++;
    }

    // if this schema is in pbest, but not in position
    if (p_count != strength) {
      for (int k = 0; k < strength; k++)
        val[k] = pbest[pos[k]];

      // new item
      PosVal pv(index, val2num(val, pos, range, strength));

      // search in velocity
      // if this schema has already in velocity, then update its probability
      auto iter = velocity.find(pv);
      if (iter != velocity.end()) {
        iter->second += factor1 * ((double) (rand() % 1000) / 2000.0);
        if (iter->second > 1.0)
          iter->second = 1.0;
      } else {
        velocity.insert(pair<PosVal, double>(pv, factor1 * ((double) (rand() % 1000) / 2000.0)));
      }
    }

    // if this schema is in gbest, but not in position
    if (g_count != strength) {
      for (int k = 0; k < strength; k++)
        val[k] = gbest[pos[k]];

      // new item
      PosVal gv(index, val2num(val, pos, range, strength));

      // search in velocity
      auto iter = velocity.find(gv);
      if (iter != velocity.end()) {
        iter->second += factor2 * ((double) (rand() % 1000) / 2000.0);
        if( iter->second > 1.0 )
        	iter->second = 1.0 ;
      } else {
        velocity.insert(pair<PosVal, double>(gv, factor2 * ((double) (rand() % 1000) / 2000.0)));
      }
    }

    // next parameter combination
    index += 1 ;
  }

  for_each(allP.begin(), allP.end(), default_delete<int>());
  allP.clear();
  delete[] val;

  /*
  cout<<"term2"<<endl;
  for( map<PosVal,double>::iterator iter = velocity.begin() ; iter != velocity.end() ; iter++ )
    cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;
  cout<<"-------------------"<<endl;
  */

  // final cut, only use items whose prob is larger than threshold
  for (auto x = velocity.begin(); x != velocity.end();) {
    if (x->second < pro1)
      x = velocity.erase(x);
    else {
      if (x->second > 1.0)
        x->second = 1.0;
      x++;
    }
  }
  /*
  cout<<"final"<<endl;
  for( map<PosVal,double>::iterator iter = velocity.begin() ; iter != velocity.end() ; iter++ )
    cout<<"("<<iter->first.posI<<", "<<iter->first.valI<<") "<<iter->second<<endl;
  cout<<"-------------------"<<endl;
  */

  // sort based on prob
  TempV.clear();
  for (auto x = velocity.begin(); x != velocity.end(); x++)
    TempV.insert(pair<double, PosVal>(x->second, x->first));

  /*
  cout<<"TempV"<<endl;
  for( set<pair<double,PosVal>>::iterator iter = TempV.begin() ; iter != TempV.end() ; iter++ )
    cout<<iter->first<<" , ("<<iter->second.posI<<", "<<iter->second.valI<<")"<<endl;
  cout<<"-------------------"<<endl;
  */
}

// ----------------------------------------------------------------------------
// update position based on TempV
// ----------------------------------------------------------------------------
void DParticle::positionUpdate(double pro2, double pro3) {

  // convergence or not
  if (velocity.size() == 0) {
    randomInit();
    return;
  }

  int *np = new int[dimension];
  for (int k = 0; k < dimension; k++)
    np[k] = -1;

  // update
  int par_count = dimension;
  for (auto iter = TempV.rbegin(); iter != TempV.rend(); iter++) {
    // select this schema with probability prob (x->first)
    if (iter->first > (double) (rand() % 1000) / 1000.0) {
      int *pos = num2pos(iter->second.posI, dimension, strength);
      int *val = num2val(iter->second.valI, pos, range, strength);

      for (int k = 0; k < strength; k++) {
        // select each value with probability pro2
        if (np[pos[k]] == -1 && (double) (rand() % 1000) / 1000.0 < pro2) {
          np[pos[k]] = val[k];
          par_count--;
        }
      }
      delete[] pos;
      delete[] val;
    }
    if (par_count == 0)
      break;
  }

  // new position
  for (int k = 0; k < dimension; k++) {
    if (np[k] != -1)
      position[k] = np[k];
  }
  delete[] np;

  // random change an element with probability pro3
  if (pro3 > (double) (rand() % 1000) / 1000.0) {
    int pa = (int) (((double) (rand() % 1000) / 1000.0) * dimension);
    int va = (int) (((double) (rand() % 1000) / 1000.0) * range[pa]);
    position[pa] = va;
  }
}

// ----------------------------------------------------------------------------
// update velocity for CLPSO
// ----------------------------------------------------------------------------
void DParticle::velocityUpdateDim(double weight, double factor, const int *clbest,
                                  int dim, double pro1) {
  // weight * velocity
  for (auto &x : velocity)
    x.second = x.second * weight;

  // construct schemas for clbest
  // for each parameter combination
  vector<int *> allP = allPos(dimension, strength);
  int *val = new int[strength];
  int index = 0;
  for (auto pos : allP) {
    // determine whether each schema is in clbest
    int cl_count = 0;
    for (int c = 0; c < strength; c++) {
      if (clbest[pos[c]] == position[pos[c]])
        cl_count++;
    }

    // if this schema is in clbest, but not in position
    if (cl_count != strength) {
      for (int k = 0; k < strength; k++)
        val[k] = pbest[pos[k]];

      // new item
      PosVal pv(index, val2num(val, pos, range, strength));

      // if this schema has already in velocity, then just update its probability
      auto iter = velocity.find(pv);
      if (iter != velocity.end()) {
        iter->second += factor * ((double) (rand() % 1000) / 2000.0);
        if (iter->second > 1.0)
          iter->second = 1.0;
      } else {
        velocity.insert(pair<PosVal, double>(pv, factor * ((double) (rand() % 1000) / 2000.0)));
      }
    }

    // next parameter combination
    index += 1 ;
  }

  for_each(allP.begin(), allP.end(), default_delete<int>());
  allP.clear();
  delete[] val;

  // final cut
  for (auto x = velocity.begin(); x != velocity.end();) {
    if (x->second < pro1)
      x = velocity.erase(x);
    else {
      if (x->second > 1.0)
        x->second = 1.0;
      x++;
    }
  }

  // sort
  TempV.clear();
  for (auto x = velocity.begin(); x != velocity.end(); x++)
    TempV.insert(pair<double, PosVal>(x->second, x->first));

}