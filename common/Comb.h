#ifndef COMB_H
#define COMB_H

#include <algorithm>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------
// C(n, m)
// ---------------------------------------------------------------------
static int combine(int n, int m) {
  int ret = 1;
  int p = n;
  for (int x = 1; x <= m; x++, p--) {
    ret = ret * p;
    ret = ret / x;
  }
  return ret;
}

// ---------------------------------------------------------------------
// the index of a given parameter combination
// ---------------------------------------------------------------------
static int pos2num(const int *c, int n, int m) {
  int ret = combine(n, m);
  for (int i = 0; i < m; i++)
    ret -= combine(n - c[i] - 1, m - i);
  ret--;
  return ret;
}

// ---------------------------------------------------------------------
// the index of a given value combination
// ---------------------------------------------------------------------
static int val2num(const int *v, const int *p, const int *value, int m)
{
  int ret = 0, com = 1 ;
  for( int k=m-1 ; k>=0 ; k-- ) {
    ret += com * v[k] ;
    com = com * value[p[k]] ;
  }
  return ret ;
}

// ---------------------------------------------------------------------
// get a parameter combination according to its index
// ---------------------------------------------------------------------
static int* num2pos(int idx, int n, int m) {
  int* pos = new int[m] ;
  idx += 1;
  int j=1, k;
  for( int i=0 ; i<m ; pos[i++]=j++ )
    for( ; idx > (k = combine(n-j, m-i-1)) ; idx -= k, j++ );
  for( int p=0 ; p<m ; p++ )
    pos[p]--;
  return pos ;
}

// ---------------------------------------------------------------------
// get a value combination according to its index
// ---------------------------------------------------------------------
static int* num2val(int idx, const int *p, const int *value, int m) {
  int *v = new int[m] ;
  int com = 1;
  for( int k=1 ; k<m ; k++ )
    com = com * value[p[k]] ;

  for( int k=0 ; k<m-1 ; k++ ) {
    v[k] = idx / com ;
    idx -= v[k] * com ;
    com = com / value[p[k+1]] ;
  }
  v[m-1] = idx ;
  return v ;
}

// ---------------------------------------------------------------------
// all parameter combinations
// ---------------------------------------------------------------------
static vector<int*> allPos(int n, int m) {
  vector<int*> data ;
  if (m == 0)
    return data;

  int* temp = new int[m];        // current combination
  int* temp_max = new int[m];    // the maximum value of each element
  for (int k = 0; k < m; k++) {
    temp[k] = k;
    temp_max[k] = n - m + k;
  }
  int end = m - 1;

  int already = 0;
  while (already < combine(n, m)) {
    // add current combination
    int* item = new int[m];
    std::copy(temp, temp+m, item);
    data.push_back(item);

    // calculate the next combination
    temp[end] = temp[end] + 1;
    int ptr = end;
    while (ptr > 0) {
      if (temp[ptr] > temp_max[ptr]) {
        temp[--ptr] += 1 ;
      } else {
        break;
      }
    }
    if (temp[ptr] <= temp_max[ptr]) {
      for (int i = ptr + 1; i < m; i++) {
        temp[i] = temp[i - 1] + 1;
      }
    }
    already++;
  }

  delete[] temp;
  delete[] temp_max;
  return data;
}

// ---------------------------------------------------------------------
// all value combinations
// ---------------------------------------------------------------------
static vector<int*> allVal(const int* p, int m, const int* value) {
  vector<int*> data ;
  if (m == 0)
    return data;

  int* counter = new int[m];         // current combination
  int* counter_max = new int[m];     // the maximum value of each element
  int comb = 1;
  for (int k = 0; k < m; k++) {
    counter[k] = 0;
    counter_max[k] = value[p[k]] - 1;
    comb = comb * value[p[k]];
  }
  int end = m - 1;

  for (int i = 0; i < comb; i++) {
    // assign data[i]
    int* item = new int[m];
    std::copy(counter, counter+m, item);
    data.push_back(item);

    // move counter to the next one
    counter[end] = counter[end] + 1;
    int ptr = end;
    while (ptr > 0) {
      if (counter[ptr] > counter_max[ptr]) {
        counter[ptr] = 0;
        counter[--ptr] += 1;
      } else
        break;
    }
  }

  delete[] counter;
  delete[] counter_max;
  return data;
}


#endif

