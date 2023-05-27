#include "includes/ACLDecompress.h"

// ACL allocator
DLLEXPORT void* nAllocate(size_t size, size_t alignment) { return ACLAllocatorImpl.allocate(size, alignment); }
DLLEXPORT void nDeallocate(void* ptr, size_t size) { ACLAllocatorImpl.deallocate(ptr, size); }

// ACL compressed tracks
DLLEXPORT const char* nCompressedTracks_IsValid(acl::compressed_tracks* tracks, bool checkHash) { return tracks->is_valid(checkHash).c_str(); }

// ACL compressed clip
DLLEXPORT const char* nCompressedClip_IsValid(void* compressedClip, uint8_t version, bool checkHash)
{
    if (version == 3)
    {
        //const acl1_2::CompressedClip* clip = static_cast<const acl1_2::CompressedClip*>(compressedClip);
        const acl1_1::CompressedClip* clip = reinterpret_cast<const acl1_1::CompressedClip*>(compressedClip);
        return clip->is_valid(checkHash).c_str();
    }
    else if (version == 4)
    {
        //const acl1_2::CompressedClip* clip = static_cast<const acl1_2::CompressedClip*>(compressedClip);
        const acl1_2::CompressedClip* clip = reinterpret_cast<const acl1_2::CompressedClip*>(compressedClip);
        return clip->is_valid(checkHash).c_str();
    }
    else if (version == 5)
    {
        //const acl1_3::CompressedClip* clip = static_cast<const acl1_3::CompressedClip*>(compressedClip);
        const acl1_3::CompressedClip* clip = reinterpret_cast<const acl1_3::CompressedClip*>(compressedClip);
        return clip->is_valid(checkHash).c_str();
    }
    else
    {
        return "Not supported";
    }
}

DLLEXPORT const int32_t nCompressedClip_GetBufferLength(void* compressedClip, uint8_t version)
{
    if (version == 3)
    {
        //const acl1_2::CompressedClip* clip = static_cast<const acl1_2::CompressedClip*>(compressedClip);
        const acl1_1::CompressedClip* clip = reinterpret_cast<const acl1_1::CompressedClip*>(compressedClip);
        const acl1_1::ClipHeader& header = get_clip_header(*clip);
        return header.num_bones * header.num_samples;
    }
    else if (version == 4)
    {
        //const acl1_2::CompressedClip* clip = static_cast<const acl1_2::CompressedClip*>(compressedClip);
        const acl1_2::CompressedClip* clip = reinterpret_cast<const acl1_2::CompressedClip*>(compressedClip);
        const acl1_2::ClipHeader& header = get_clip_header(*clip);
        return header.num_bones * header.num_samples;
    }
    else if (version == 5)
    {
        //const acl1_3::CompressedClip* clip = static_cast<const acl1_3::CompressedClip*>(compressedClip);
        const acl1_3::CompressedClip* clip = reinterpret_cast<const acl1_3::CompressedClip*>(compressedClip);
        const acl1_3::ClipHeader& header = get_clip_header(*clip);
        return header.num_bones * header.num_samples;
    }
    else
    {
        return 0;
    }
}


DLLEXPORT void nReadACLData(const acl::compressed_tracks& tracks, FVector* outPosKeys, FQuat* outRotKeys, FVector* outScaleKeys)
{
    uint32_t numSamples = tracks.get_num_samples_per_track();
    float sampleRate = tracks.get_sample_rate();
    float duration = tracks.get_finite_duration();

    DecompContextDefault context;
    context.initialize(tracks);

    FCUE4ParseOutputWriter writer(outPosKeys, outRotKeys, outScaleKeys, numSamples);
    for (uint32_t sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        const float sample_time = rtm::scalar_min(float(sampleIndex) / sampleRate, duration);
        context.seek(sample_time, acl::sample_rounding_policy::nearest);
        writer.SampleIndex = sampleIndex;
        context.decompress_tracks(writer);
    }
}

void nReadACLClipData1_1(const acl1_1::CompressedClip& clip, FVector* outPosKeys, FQuat* outRotKeys, FVector* outScaleKeys)
{
    const acl1_1::ClipHeader& header = get_clip_header(clip);
    const uint32_t numSamples = header.num_samples;
    const uint32_t sampleRate = header.sample_rate;
    const float duration = acl1_1::calculate_duration(header.num_samples, header.sample_rate);
    DecompContext1_1 context;
    context.initialize(clip);

    FCUE4ParseOutputWriter1_1 writer(outPosKeys, outRotKeys, outScaleKeys, numSamples);
    for (uint32_t sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        const float sample_time = rtm::scalar_min(float(sampleIndex) / sampleRate, duration);
        context.seek(sample_time, acl1_1::SampleRoundingPolicy::Nearest);
        writer.SampleIndex = sampleIndex;
        context.decompress_pose(writer);
    }
}

void nReadACLClipData1_2(const acl1_2::CompressedClip& clip, FVector* outPosKeys, FQuat* outRotKeys, FVector* outScaleKeys)
{
    const acl1_2::ClipHeader& header = get_clip_header(clip);
    const uint32_t numSamples = header.num_samples;
    const float sampleRate = header.sample_rate;
    const float duration = acl1_2::calculate_duration(header.num_samples, header.sample_rate);
    DecompContext1_2 context;
    context.initialize(clip);

    FCUE4ParseOutputWriter1_2 writer(outPosKeys, outRotKeys, outScaleKeys, numSamples);
    for (uint32_t sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        const float sample_time = rtm::scalar_min(float(sampleIndex) / sampleRate, duration);
        context.seek(sample_time, acl1_2::SampleRoundingPolicy::Nearest);
        writer.SampleIndex = sampleIndex;
        context.decompress_pose(writer);
    }
}

void nReadACLClipData1_3(const acl1_3::CompressedClip& clip, FVector* outPosKeys, FQuat* outRotKeys, FVector* outScaleKeys)
{
    const acl1_3::ClipHeader& header = get_clip_header(clip);
    const uint32_t numSamples = header.num_samples;
    const float sampleRate = header.sample_rate;
    const float duration = acl1_3::calculate_duration(header.num_samples, header.sample_rate);
    DecompContext1_3 context;
    context.initialize(clip);

    FCUE4ParseOutputWriter1_3 writer(outPosKeys, outRotKeys, outScaleKeys, numSamples);
    for (uint32_t sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        const float sample_time = rtm::scalar_min(float(sampleIndex) / sampleRate, duration);
        context.seek(sample_time, acl1_3::SampleRoundingPolicy::Nearest);
        writer.SampleIndex = sampleIndex;
        context.decompress_pose(writer);
    }
}

DLLEXPORT void nReadACLClipData(const void* compressedClip, uint8_t version, FVector* outPosKeys, FQuat* outRotKeys, FVector* outScaleKeys)
{
    if (version == 3)
    {
        const acl1_1::CompressedClip* clip = static_cast<const acl1_1::CompressedClip*>(compressedClip);
        nReadACLClipData1_1(*clip, outPosKeys, outRotKeys, outScaleKeys);
    }
    else if (version == 4)
    {
        const acl1_2::CompressedClip* clip = static_cast<const acl1_2::CompressedClip*>(compressedClip);
        nReadACLClipData1_2(*clip, outPosKeys, outRotKeys, outScaleKeys);
    }
    else if (version == 5)
    {
        const acl1_3::CompressedClip* clip = static_cast<const acl1_3::CompressedClip*>(compressedClip);
        nReadACLClipData1_3(*clip, outPosKeys, outRotKeys, outScaleKeys);
    }
    else
    {

    }
}
