using System;
using System.Runtime.InteropServices;
using static CUE4Parse.ACL.ACLNative;

namespace CUE4Parse.ACL
{
    public class CompressedClip
    {
        public IntPtr Handle { get; private set; }
        private readonly int _bufferLength;

        public CompressedClip(byte version, byte[] buffer)
        {
            _bufferLength = buffer.Length;
            Handle = nAllocate(_bufferLength);
            Marshal.Copy(buffer, 0, Handle, buffer.Length);
            var error = IsValid(version, false);
            if (error != null)
            {
                nDeallocate(Handle, _bufferLength);
                Handle = IntPtr.Zero;
                throw new ACLException(error);
            }
        }

        public CompressedClip(IntPtr existing)
        {
            _bufferLength = -1;
            Handle = existing;
        }

        ~CompressedClip()
        {
            if (_bufferLength >= 0 && Handle != IntPtr.Zero)
            {
                nDeallocate(Handle, _bufferLength);
                Handle = IntPtr.Zero;
            }
        }

        public int GetBufferLength(byte version) => nCompressedClip_GetBufferLength(Handle, version);

        public string? IsValid(byte version, bool checkHash)
        {
            var error = version switch
            {
                > 2 and <= 5 => Marshal.PtrToStringAnsi(nCompressedClip_IsValid(Handle, version, checkHash))!,
                _ => "Older ACL versions aren't supported"
            };
            return error.Length > 0 ? error : null;
        }

        [DllImport(LIB_NAME)]
        private static extern IntPtr nCompressedClip_IsValid(IntPtr handle, byte version, bool checkHash);
        [DllImport(LIB_NAME)]
        private static extern int nCompressedClip_GetBufferLength(IntPtr handle, byte version);
    }
}
