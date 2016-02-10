#ifndef FITNESS_EVALUATOR_H
#define FITNESS_EVALUATOR_H

/*==============================================================================
  Compares chess evaluation functions (encoded as chromosomes) in order to find
  the best ones when running a genetic algorithm
 ==============================================================================*/

#include <string>
#include "Util.h"

class Search;
class Board;
class Chromosome;
class PositionEvaluator;

class FitnessEvaluator
{
 public:
   FitnessEvaluator (Search* engine);
   ~FitnessEvaluator ();

   double evaluate (Chromosome& a, Chromosome& b);
   static const uint MAX_ALLOWED_MOVEMENTS = 70;

 private:
   Search* engine;
   Board* board;
   PositionEvaluator* evaluator;
};

#endif // FITNESS_FUNCTION_H
