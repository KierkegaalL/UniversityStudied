package TransChat;

import WebTranslator.WebTranslator;
import java.io.*;
import java.net.*;

/**
 * クライアント通信処理スレッド
 * @author 学籍番号 15024031　氏名　奥 龍司// 自分の氏名・番号を記入して下さい
 */
public class MyClient extends Thread {
    // 親のMyChatへの参照
    MyChat parent;
    
    // チャットアプリ用のポート番号
    int port = 13579;
    
    // 状態管理用フラグ
    private boolean status = true;

    // サーバとの通信用のソケット
    Socket socket;

    //===========================================
    // 各種リーダ、ライター、ストリーム
    //===========================================
    
    // ソケットからの入出力
    public BufferedReader in = null; // リーダー(入力インタフェース)
    public PrintWriter out = null;   // ライター(出力インタフェース)

    //================================================================
    /**
     * 初期化メソッド
     * @param parent   メインオブジェクトへの参照
     * @param hostname 接続するサーバのホスト名
     * @param portno   接続するサーバのポート番号
     * @throws Exception 
     */
    public MyClient(MyChat parent, String hostname, int portno) throws Exception {
        /*
         * throws Exception を付けることで、このメソッド内で発生した
         * Exceptionはすべて呼出元で処理するように上に投げられる
        */
        
        // 親のMyChatへの参照を受け取る
        this.parent = parent; 
        
        // ポート番号の取得(設定画面等で変更可能とするための措置)
        port = portno;

        // サーバに接続する
        socket = new Socket(hostname, port);
        
        //-----------------------------------------------------
        // ソケットから入出力インタフェースを取得する
        //-----------------------------------------------------
        // リーダーは使いやすいようにBufferedReaderにラップする
        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        // ライターも使いやすいようにPrintWriterにラップする
        out = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
    }

    //================================================================
    /**
     * スレッドのメイン処理部
     */
    @Override
    public void run() {
        while (status) {
            try {
//******************************************************************
                // サーバからのメッセージを１行読み込み出力画面に表示する
                // ※readLine()は改行でメッセージを区切って読込み，改行は破棄されるので
                // 　表示のために改めて末尾に追加している
                String message = in.readLine() + "\n";
                //System.out.println(message);
                String str[] = message.split("\t");
                WebTranslator wt = new WebTranslator();
                String translated = wt.translation(str[1].trim(),0,1);   
                message = "["+str[0]+"] "+translated+" (元文:"+str[1].trim()+")";
                // 親画面のテキストエリアにメッセージを追記する
                parent.append(message);
//******************************************************************
            } catch (IOException e) {
                // 通信エラーなどが発生したらこの処理に入るので、スレッドを停止させる
                status = false;
            }
        }
        // メインループの後処理
        parent.append("サーバとの通信を停止しました.\n");
        // 終了処理
        close();
    }

    //================================================================
    /**
     * 出力用の通信インタフェースを提供するゲッターメソッド
     * @return 出力用のライタ
     */
    public PrintWriter getWriter() {
        // Writerを返す
        return out;
    }

    //================================================================
    /**
     * 終了処理
     */
    public void close() {
        // スレッドを停止
        status = false;
        // null にすることで参照を切り、GCにオブジェクトを廃棄してもらう
        in = null;
        out = null;
        // ソケット停止
        try{
            if(socket != null){
                socket.close(); // ソケットを閉じる
                socket = null;  // オブジェクトを廃棄する
            }
        }catch(IOException e){
            // 切断に失敗した場合の対処を書く部分
        }
    }
}
