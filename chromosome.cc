/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  assert(is_valid());
  static std::random_device dev; //obtain seed for random number generator
  static std::mt19937 rng(dev());
  int first_index; //will be switching values with second_index
  int second_index;
  int first_tmp; //temporary place to put the values while transferring
  int second_tmp;
  std::uniform_int_distribution<std::mt19937::result_type> distrib(0,order_.size()); // distribution in range of chromosome
  first_index = distrib(rng);
  second_index = distrib(rng);
  while (second_index == first_index){ //make sure reset second_index if it equals first_index
    second_index = distrib(rng);
  }
  first_tmp = order_[first_index]; //switch values
  second_tmp = order_[second_index];
  order_[first_index] = second_tmp;
  order_[second_index] = first_tmp;
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());

  //create subset
  static std::random_device dev; //obtain seed for random number generator
  static std::mt19937 rng(dev());
  int b; //begin
  int e;
  std::uniform_int_distribution<std::mt19937::result_type> distrib(0,(order_.size()-1); // distribution in range of chromosome
  b = distrib(rng);
  e = distrib(rng);
  while (b == (order_.size()-1)){ //make sure b is not the last element
      b = distrib(rng);
  }
  while (b > e){ //this is to make sure end comes after beginning
      e = distrib(rng);
  }
  
  Chromosome* first_born = Chromosome::create_crossover_child(*this, other, b, e);
  Chromosome* second_born = Chromosome::create_crossover_child(other, *this, b, e);
  auto siblings = std::make_pair(first_born, second_born);
  return siblings;
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
        assert(j < p2->order_.size());
      }
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
  return (1/cities_ptr_.total_path_distance(order_t&)); // results will be doubles from 0 to 1, larger means shorter traversal path
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
bool
Chromosome::is_valid() const
{
  for (int i = 0; i<order.size(); i++){
    if (order[i]>=order.size()){ //not valid if an index is above range of chromosome
      return false;
    }
    if ((std::count (order.begin(), (order.end() - 1), i)=0 || (std::count (order.begin(), (order.end() - 1), i)>1))) { //not valid if there is a repeated value, count checks how many times that number appears
      return false;
    }
  }
  return true; //if no indices set that off then we're good to go
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  assert end>begin;
  for (int i = begin, i<=end, i++){
    if (order[i] == value){
      return true;
    }
  }
  return false;
}
