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
    //      メモリ・ストリーム
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

        enum StreamStatus : int
        {
            NoError,
            Disconnected
        }
        void CheckError(StreamStatus iStreamStatus)
        {
            switch(iStreamStatus)
            {
            case StreamStatus.Disconnected:
                throw new ObjectDisposedException("theolizer : CppOStream disconnected");
            }
        }

        //----------------------------------------------------------------------------
        //      C# → C++転送
        //----------------------------------------------------------------------------

        [DllImport("cpp_server.dll")]
        extern static StreamStatus CppWrite(IntPtr handle, IntPtr buffer, int offset, int count);

        [DllImport("cpp_server.dll")]
        extern static StreamStatus CppFlush(IntPtr handle);

        public override void Write(byte[] buffer, int offset, int count)
        {
            GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            StreamStatus ret=CppWrite(mCppHandle, handle.AddrOfPinnedObject(), offset, count);
            handle.Free();
            CheckError(ret);
        }

        public override void Flush()
        {
            StreamStatus ret=CppFlush(mCppHandle);
            CheckError(ret);
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
            throw new NotImplementedException();
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            throw new NotImplementedException();
        }

        public override long Length
        {
            get
            {
                throw new NotImplementedException();
            }
        }

        public override long Position
        {
            get
            {
                throw new NotImplementedException();
            }

            set
            {
                throw new NotImplementedException();
            }
        }

        public override void SetLength(long value)
        {
            throw new NotImplementedException();
        }
    }
}
