//############################################################################
//      データ交換用メモリ・ストリーム(C#側)
/*
    © 2016 Theoride Technology (http://theolizer.com/) All Rights Reserved.
    "Theolizer" is a registered trademark of Theoride Technology.

    "Theolizer" License
        In the case where you are in possession of a valid “Theolizer” License,
        you may use this file in accordance with the terms and conditions of 
        the use license determined by Theoride Technology.

    General Public License Version 3 ("GPLv3")
        You may use this file in accordance with the terms and conditions of 
        GPLv3 published by Free Software Foundation.
        Please confirm the contents of GPLv3 at https://www.gnu.org/licenses/gpl.txt .
        A copy of GPLv3 is also saved in a LICENSE.TXT file.

    商用ライセンス
        あなたが有効なTheolizer商用ライセンスを保持している場合、
        セオライド テクノロジーの定める使用許諾書の条件に従って、
        このファイルを取り扱うことができます。

    General Public License Version 3(以下GPLv3)
        Free Software Foundationが公表するGPLv3の使用条件に従って、
        あなたはこのファイルを取り扱うことができます。
        GPLv3の内容を https://www.gnu.org/licenses/gpl.txt にて確認して下さい。
        またGPLv3のコピーをLICENSE.TXTファイルにおいてます。
*/
//############################################################################

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace theolizer
{
    // ***************************************************************************
    //      ストリーム状態管理
    // ***************************************************************************

    class CppDisconnected : Exception
    {
        public CppDisconnected(String message) : base(message)
        {
        }
    }

    enum StreamStatus : int
    {
        NoError,
        Disconnected
    }

    class StreamStatusMan
    {
        public static void CheckError(StreamStatus iStreamStatus)
        {
            switch(iStreamStatus)
            {
            case StreamStatus.Disconnected:
                throw new CppDisconnected("theolizer : CppOStream disconnected");
            }
        }
    }

    // ***************************************************************************
    //      C# → C++メモリ・ストリーム
    // ***************************************************************************

    class CppOStream : Stream
    {
        //----------------------------------------------------------------------------
        //      管理領域
        //----------------------------------------------------------------------------

        IntPtr  mCppHandle;
        public CppOStream(IntPtr iCppHandle)
        {
            mCppHandle = iCppHandle;
        }

        //----------------------------------------------------------------------------
        //      C# → C++転送
        //----------------------------------------------------------------------------

        [DllImport(Constants.CppDllName)]
        extern static StreamStatus CppWrite(IntPtr handle, IntPtr buffer, int offset, int count);

        [DllImport(Constants.CppDllName)]
        extern static StreamStatus CppFlush(IntPtr handle);

        public override void Write(byte[] buffer, int offset, int count)
        {
            GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            StreamStatus ret=CppWrite(mCppHandle, handle.AddrOfPinnedObject(), offset, count);
            handle.Free();
            StreamStatusMan.CheckError(ret);
        }

        public override void Flush()
        {
            StreamStatus ret=CppFlush(mCppHandle);
            StreamStatusMan.CheckError(ret);
        }

        public override bool CanWrite
        {
            get { return true; }
        }

        public override bool CanRead
        {
            get { return false; }
        }

        public override bool CanSeek
        {
            get { return false; }
        }

        //----------------------------------------------------------------------------
        //      非サポート・メソッド群
        //----------------------------------------------------------------------------

        public override int Read(byte[] buffer, int offset, int count)
        {
            throw new NotSupportedException();
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            throw new NotSupportedException();
        }

        public override long Length
        {
            get
            {
                throw new NotSupportedException();
            }
        }

        public override long Position
        {
            get
            {
                throw new NotSupportedException();
            }

            set
            {
                throw new NotSupportedException();
            }
        }

        public override void SetLength(long value)
        {
            throw new NotSupportedException();
        }
    }

    // ***************************************************************************
    //      C++ → C#メモリ・ストリーム
    // ***************************************************************************

    class CppIStream : Stream
    {
        //----------------------------------------------------------------------------
        //      管理領域
        //----------------------------------------------------------------------------

        IntPtr  mCppHandle;
        public CppIStream(IntPtr iCppHandle)
        {
            mCppHandle = iCppHandle;
        }

        //----------------------------------------------------------------------------
        //      C++ → C#転送
        //----------------------------------------------------------------------------

        [DllImport(Constants.CppDllName)]
        extern static StreamStatus CppRead
        (
            IntPtr handle,
            IntPtr buffer,
            int offset,
            int count,
            ref int out_count
        );

        public override int Read(byte[] buffer, int offset, int count)
        {
            int out_count = 0;
            GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            StreamStatus ret=CppRead(mCppHandle, handle.AddrOfPinnedObject(), offset, count,ref out_count);
            handle.Free();
            StreamStatusMan.CheckError(ret);

            return out_count;
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            throw new NotSupportedException();
        }

        public override bool CanWrite
        {
            get { return false; }
        }

        public override bool CanRead
        {
            get { return true; }
        }

        public override bool CanSeek
        {
            get { return false; }
        }

        //----------------------------------------------------------------------------
        //      非サポート・メソッド群
        //----------------------------------------------------------------------------

        public override void Write(byte[] buffer, int offset, int count)
        {
            throw new NotSupportedException();
        }

        public override void Flush()
        {
            throw new NotSupportedException();
        }

        public override long Length
        {
            get
            {
                throw new NotSupportedException();
            }
        }

        public override long Position
        {
            get
            {
                throw new NotSupportedException();
            }

            set
            {
                throw new NotSupportedException();
            }
        }

        public override void SetLength(long value)
        {
            throw new NotSupportedException();
        }
    }
}
