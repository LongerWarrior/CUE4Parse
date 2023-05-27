using System.Runtime.CompilerServices;

namespace CUE4Parse.ACL
{
    public enum buffer_tag32 : uint
    {
        //////////////////////////////////////////////////////////////////////////
        // Identifies a 'CompressedClip' buffer.
        // Deprecated, no longer used. Belonged to pre-2.0 file format.
        compressed_clip = 0xac10ac10,

        //////////////////////////////////////////////////////////////////////////
        // Identifies a 'compressed_tracks' buffer.
        compressed_tracks = 0xac11ac11,

        //////////////////////////////////////////////////////////////////////////
        // Identifies a 'compressed_database' buffer.
        compressed_database = 0xac11db01,
    };

    public struct RawBufferHeader
    {
        public uint Size;
        public uint Hash;
    }

    public struct TracksHeader
    {
        public uint Tag;
        public ushort Version;
        public byte AlgorithmType;
        public byte TrackType;
        public uint NumTracks;
        public uint NumSamples;
        public float SampleRate;
        public uint MiscPacked;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool GetHasScale() => (MiscPacked & 1) != 0;
    }
}