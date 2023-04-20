#define BENCHMARK_MAP( n ) \
{ \
  map_##n##_insert_nonexisting_result.set_active_plot( MAP_ID ); \
  map_##n##_erase_existing_result.set_active_plot( MAP_ID ); \
  map_##n##_insert_existing_result.set_active_plot( MAP_ID ); \
  map_##n##_erase_nonexisting_result.set_active_plot( MAP_ID ); \
  map_##n##_get_existing_result.set_active_plot( MAP_ID ); \
  map_##n##_get_nonexisting_result.set_active_plot( MAP_ID ); \
  map_##n##_iteration_result.set_active_plot( MAP_ID ); \
  \
  std::chrono::time_point<std::chrono::high_resolution_clock> start; \
  \
  /* Insert nonexisting and erase existing */ \
  if( BENCH_INSERT_NONEXISTING ) \
  { \
    MAP_##n##_INIT; \
    std::this_thread::sleep_for( std::chrono::milliseconds( MS_WAIT_BETWEEN_BENCHMARKS ) ); \
    \
    start = std::chrono::high_resolution_clock::now(); \
    \
    for( size_t i = 0, j = 0; i < TOTAL_ELEMENTS; ) \
    { \
      MAP_##n##_INSERT( map_##n##_keys_for_insert[ i ], map_##n##_el_ty() ); \
      \
      ++i; \
      if( ++j == MEASUREMENT_INTERVAL ) \
      { \
        map_##n##_insert_nonexisting_result.record_time( \
          run, \
          i / MEASUREMENT_INTERVAL - 1, \
          std::chrono::duration_cast<std::chrono::microseconds>( \
            std::chrono::high_resolution_clock::now() - start \
          ).count() \
        ); \
        j = 0; \
      } \
    } \
    \
    MAP_##n##_CLEANUP; \
  } \
  \
  /* Erase existing */ \
  if( BENCH_ERASE_EXISTING ){ \
    MAP_##n##_INIT; \
    std::this_thread::sleep_for( std::chrono::milliseconds( MS_WAIT_BETWEEN_BENCHMARKS ) ); \
    \
    for( size_t i = 0, j = 0; i < TOTAL_ELEMENTS; ) \
    { \
      MAP_##n##_INSERT( map_##n##_keys_for_insert[ i ], map_##n##_el_ty() ); \
      \
      ++i; \
      if( ++j == MEASUREMENT_INTERVAL ) \
      { \
        j = 0; \
        \
        start = std::chrono::high_resolution_clock::now(); \
        \
        size_t keys_start = std::uniform_int_distribution<size_t>( 0, i - 1 )( rng ); \
        \
        for( size_t k = 0, l = keys_start; k < 1000; ++k ) \
        { \
          MAP_##n##_ERASE( map_##n##_keys_for_insert[ l ] ); \
          if( ++l == i ) \
            l = 0; \
        } \
        \
        map_##n##_erase_existing_result.record_time( \
          run, \
          i / MEASUREMENT_INTERVAL - 1, \
          std::chrono::duration_cast<std::chrono::microseconds>( \
            std::chrono::high_resolution_clock::now() - start \
          ).count() \
        ); \
        \
        for( size_t k = 0, l = keys_start; k < 1000; ++k ) \
        { \
          MAP_##n##_INSERT( map_##n##_keys_for_insert[ l ], map_##n##_el_ty() ); \
          if( ++l == i ) \
            l = 0; \
        } \
      } \
    } \
    \
    MAP_##n##_CLEANUP; \
  } \
  \
  /* Insert existing, get existing, get nonexisting, and erase nonexisting */ \
  { \
    MAP_##n##_INIT; \
    std::this_thread::sleep_for( std::chrono::milliseconds( MS_WAIT_BETWEEN_BENCHMARKS ) ); \
    \
    volatile unsigned long long total = 0; \
    \
    for( size_t i = 0, j = 0; i < TOTAL_ELEMENTS; ) \
    { \
      MAP_##n##_INSERT( map_##n##_keys_for_insert[ i ], map_##n##_el_ty() ); \
      \
      ++i; \
      if( ++j == MEASUREMENT_INTERVAL ) \
      { \
        j = 0; \
        \
        /* Insert existing */ \
        if( BENCH_INSERT_EXISTING ) \
        { \
          start = std::chrono::high_resolution_clock::now(); \
          \
          for( size_t k = 0, l = std::uniform_int_distribution<size_t>( 0, i - 1 )( rng ); k < 1000; ++k ) \
          { \
            MAP_##n##_INSERT( map_##n##_keys_for_insert[ l ], map_##n##_el_ty() ); \
            if( ++l == i ) \
              l = 0; \
          } \
          \
          map_##n##_insert_existing_result.record_time( \
            run, \
            i / MEASUREMENT_INTERVAL - 1, \
            std::chrono::duration_cast<std::chrono::microseconds>( \
              std::chrono::high_resolution_clock::now() - start \
            ).count() \
          ); \
        } \
        \
        /* Get existing */ \
        if( BENCH_GET_EXISTING ) \
        { \
          start = std::chrono::high_resolution_clock::now(); \
          \
          for( size_t k = 0, l = std::uniform_int_distribution<size_t>( 0, i - 1 )( rng ); k < 1000; ++k ) \
          { \
            total += MAP_##n##_GET( map_##n##_keys_for_insert[ l ] ); \
            if( ++l == i ) \
              l = 0; \
          } \
          \
          map_##n##_get_existing_result.record_time( \
            run, \
            i / MEASUREMENT_INTERVAL - 1, \
            std::chrono::duration_cast<std::chrono::microseconds>( \
              std::chrono::high_resolution_clock::now() - start \
            ).count() \
          ); \
        } \
        \
        /* Get non-existing */ \
        if( BENCH_GET_NONEXISTING ) \
        { \
          start = std::chrono::high_resolution_clock::now(); \
          \
          for( size_t k = 0, l = std::uniform_int_distribution<size_t>( 0, TOTAL_ELEMENTS - 1 )( rng ); k < 1000; ++k ) \
          { \
            total += MAP_##n##_GET( map_##n##_keys_nonexisting[ l ] ); \
            if( ++l == TOTAL_ELEMENTS ) \
              l = 0; \
          } \
          \
          map_##n##_get_nonexisting_result.record_time( \
            run, \
            i / MEASUREMENT_INTERVAL - 1, \
            std::chrono::duration_cast<std::chrono::microseconds>( \
              std::chrono::high_resolution_clock::now() - start \
            ).count() \
          ); \
        } \
        \
        /* Erase non-existing */ \
        \
        if( BENCH_ERASE_NONEXISTING ) \
        { \
          start = std::chrono::high_resolution_clock::now(); \
          \
          for( size_t k = 0, l = std::uniform_int_distribution<size_t>( 0, TOTAL_ELEMENTS - 1 )( rng ); k < 1000; ++k ) \
          { \
            MAP_##n##_ERASE( map_##n##_keys_nonexisting[ l ] ); \
            if( ++l == TOTAL_ELEMENTS ) \
              l = 0; \
          } \
          \
          map_##n##_erase_nonexisting_result.record_time( \
            run, \
            i / MEASUREMENT_INTERVAL - 1, \
            std::chrono::duration_cast<std::chrono::microseconds>( \
              std::chrono::high_resolution_clock::now() - start \
            ).count() \
          ); \
        } \
      } \
    } \
    \
    MAP_##n##_CLEANUP; \
  } \
} \


std::cout << "  " << MAP_ID << "\n";
BENCHMARK_MAP( 1 );
BENCHMARK_MAP( 2 );
BENCHMARK_MAP( 3 );

#undef MAP_ID
#undef MAP_COLOR
#undef MAP_1_INIT
#undef MAP_2_INIT
#undef MAP_3_INIT
#undef MAP_1_INSERT
#undef MAP_2_INSERT
#undef MAP_3_INSERT
#undef MAP_1_GET
#undef MAP_2_GET
#undef MAP_3_GET
#undef MAP_1_ERASE
#undef MAP_2_ERASE
#undef MAP_3_ERASE
#undef MAP_1_CLEANUP
#undef MAP_2_CLEANUP
#undef MAP_3_CLEANUP

/*

    \
    start = std::chrono::high_resolution_clock::now(); \
    \
    for( size_t i = 0, j = 0; i < TOTAL_ELEMENTS; ) \
    { \
      MAP_##n##_ERASE( map_##n##_keys_for_erase[ i ] ); \
      \
      ++i; \
      if( ++j == MEASUREMENT_INTERVAL ) \
      { \
        map_##n##_erase_existing_result.record_time( \
          run, \
          i / MEASUREMENT_INTERVAL - 1, \
          std::chrono::duration_cast<std::chrono::microseconds>( \
            std::chrono::high_resolution_clock::now() - start \
          ).count() \
        ); \
        \
        j = 0; \
      } \
    } \
*/
