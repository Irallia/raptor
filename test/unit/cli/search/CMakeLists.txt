# --------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/raptor/blob/main/LICENSE.md
# --------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.18)

raptor_add_unit_test (search_hibf_test.cpp)
raptor_add_unit_test (search_hibf_preprocessing_test.cpp)
raptor_add_unit_test (search_ibf_preprocessing_test.cpp)
raptor_add_unit_test (search_ibf_test.cpp)
