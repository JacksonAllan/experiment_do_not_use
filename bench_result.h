#ifndef BENCH_RESULT_H
#define BENCH_RESULT_H

#include <string>
#include <utility>
#include <vector>

class bench_result_ty
{
public:

  bench_result_ty( size_t num_runs, size_t num_recordings_per_run )
  : num_runs( num_runs ), num_recordings_per_run( num_recordings_per_run )
  {};

  void set_active_plot( std::string plot_id );

  void record_time( size_t run, size_t recording, unsigned long long time );

  std::string to_svg( std::string heading, std::string x_axis_label, unsigned long long datapoint_interval );

private:

  size_t num_runs;
  size_t num_recordings_per_run;
  std::vector<std::pair<std::string, std::vector<unsigned long long>>> plots;
  size_t active_plot;

  double get_average_time( size_t plot, size_t recording );
};

#endif