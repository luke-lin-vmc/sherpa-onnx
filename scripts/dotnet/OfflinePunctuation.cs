﻿/// Copyright (c)  2024  Xiaomi Corporation (authors: Fangjun Kuang)
using System;
using System.Runtime.InteropServices;
using System.Text;


namespace SherpaOnnx
{
    public class OfflinePunctuation : IDisposable
    {
        public OfflinePunctuation(OfflinePunctuationConfig config)
        {
            IntPtr h = SherpaOnnxCreateOfflinePunctuation(ref config);
            _handle = new HandleRef(this, h);
        }

        public String AddPunct(String text)
        {
            byte[] utf8Bytes = Encoding.UTF8.GetBytes(text);
            byte[] utf8BytesWithNull = new byte[utf8Bytes.Length + 1]; // +1 for null terminator
            Array.Copy(utf8Bytes, utf8BytesWithNull, utf8Bytes.Length);
            utf8BytesWithNull[utf8Bytes.Length] = 0; // Null terminator

            IntPtr p = SherpaOfflinePunctuationAddPunct(_handle.Handle, utf8BytesWithNull);

            string s = "";
            int length = 0;

            unsafe
            {
                byte* b = (byte*)p;
                if (b != null)
                {
                    while (*b != 0)
                    {
                        ++b;
                        length += 1;
                    }
                }
            }

            if (length > 0)
            {
                byte[] stringBuffer = new byte[length];
                Marshal.Copy(p, stringBuffer, 0, length);
                s = Encoding.UTF8.GetString(stringBuffer);
            }

            SherpaOfflinePunctuationFreeText(p);

            return s;
        }

        public void Dispose()
        {
            Cleanup();
            // Prevent the object from being placed on the
            // finalization queue
            System.GC.SuppressFinalize(this);
        }

        ~OfflinePunctuation()
        {
            Cleanup();
        }

        private void Cleanup()
        {
            SherpaOnnxDestroyOfflinePunctuation(_handle.Handle);

            // Don't permit the handle to be used again.
            _handle = new HandleRef(this, IntPtr.Zero);
        }

        private HandleRef _handle;


        [DllImport(Dll.Filename)]
        private static extern IntPtr SherpaOnnxCreateOfflinePunctuation(ref OfflinePunctuationConfig config);

        [DllImport(Dll.Filename)]
        private static extern void SherpaOnnxDestroyOfflinePunctuation(IntPtr handle);

        [DllImport(Dll.Filename)]
        private static extern IntPtr SherpaOfflinePunctuationAddPunct(IntPtr handle, [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)] byte[] utf8Text);

        [DllImport(Dll.Filename)]
        private static extern void SherpaOfflinePunctuationFreeText(IntPtr p);
    }
}

