#pragma once
#include <acl/core/ansi_allocator.h>
#include <acl/core/compressed_tracks.h>
#include <acl/decompression/decompress.h>

//#include <acl1_3/core/ansi_allocator.h>
#include <acl1_3/core/compressed_clip.h>
#include <acl1_3/core/utils.h>
#include <acl1_3/algorithm/uniformly_sampled/decoder.h>

//#include <acl1_2/core/ansi_allocator.h>
#include <acl1_2/core/compressed_clip.h>
#include <acl1_2/core/utils.h>
#include <acl1_2/algorithm/uniformly_sampled/decoder.h>

//#include <acl1_1/core/ansi_allocator.h>
#include <acl1_1/core/compressed_clip.h>
#include <acl1_1/core/utils.h>
#include <acl1_1/algorithm/uniformly_sampled/decoder.h>

#include "Structs.h"
#include "Framework.h"

acl::ansi_allocator ACLAllocatorImpl;
using DecompContextDefault = acl::decompression_context<acl::decompression_settings>;
using DecompContext1_3 = acl1_3::uniformly_sampled::DecompressionContext<acl1_3::uniformly_sampled::DecompressionSettings>;
using DecompContext1_2 = acl1_2::uniformly_sampled::DecompressionContext<acl1_2::uniformly_sampled::DecompressionSettings>;
using DecompContext1_1 = acl1_1::uniformly_sampled::DecompressionContext<acl1_1::uniformly_sampled::DecompressionSettings>;
