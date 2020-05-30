#include <Rcpp.h>

// [[Rcpp::plugins(cpp17)]]

#include <epideux/epideux.h>

using namespace epideux;
using namespace std::chrono_literals;


void vprint(std::vector<uint32_t> const &input) {
  for (int i = 0; i < input.size(); i++) {
    Rcpp::Rcout << input.at(i) << ' ';
  }
  Rcpp::Rcout << std::endl;
}

// [[Rcpp::export]]
void test(int persons = 100, int days = 10)
{
  Rcpp::Rcout << "Small simulation:" << std::endl;
  
  Model sim_model;
  sim_model.setStartDate(2020, 4, 1);
  
  auto school = sim_model.createLocation(0.7);
  
  time_pt start_time;
  time_pt end_time = start_time + 2h;
  ItineraryEntry school_it(school, start_time, end_time);
  
  for (int i = 0; i < persons; ++i) {
    auto my_home = sim_model.createLocation(0.7);
    auto my_person = sim_model.createPerson(my_home);
    my_person->addItineraryEntry(school_it);
  }
  
  sim_model.simulate(24h*days);
  
  SeirReport rep = sim_model.getReport();
  
  Rcpp::Rcout << "Susceptible:" << std::endl;
  vprint(rep.susceptible);
  Rcpp::Rcout << "Exposed:" << std::endl;
  vprint(rep.exposed);
  Rcpp::Rcout << "Infectious:" << std::endl;
  vprint(rep.infectious);
  Rcpp::Rcout << "Recovered:" << std::endl;
  vprint(rep.recovered);
  
  Rcpp::Rcout << "Done." << std::endl;
}
