package appexercise01;

import javafx.application.Application;
import static javafx.application.Application.launch;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;

/**
 * オブジェクト指向応用演習　前半課題
 * 電卓プログラムのメインクラス
 * @author 学籍番号　氏名　// 自分の氏名・番号を記入して下さい
 */
public class MyCalc extends Application {
    // 計算クラス本体
    Calculator calc;
    
    // 一行に配置するボタンの数
    int cols = 4;
    // ボタンの名称配列 ※この並びで表示される
    String bname[] = {
        "AC", "C", "%","/",
        "7" , "8", "9","*",
        "4" , "5", "6","-",
        "1" , "2", "3","+",
        "0" , "±", ".","="};

    //-------------------------------------------------------------------------
    /**
     * 処理部メイン
     * @param stage 
     */
    @Override
    public void start(Stage stage) {
        // ウィンドウタイトルを設定
        stage.setTitle("JavaFX電卓");
        // ウィンドウの幅設定
        stage.setWidth(380);              
        // ウィンドウの高さ設定
        stage.setHeight(600);
        
        // 計算結果出力画面
        TextField win = new TextField("0"); // 0は初期表示
        
        // 計算機本体のオブジェクトを生成
        calc = new Calculator(win); // calc側から結果を表示できるように出力画面の参照を渡す

        // ボタンを格子状に配置する
        GridPane btns = new GridPane();
        // ボタン領域の外枠を上下左右10ずつ余白を取る
        btns.setPadding(new Insets(10,10,10,10));
        // ボタン領域のサイズを固定。余白を含むので実際の内部サイズは(240,240)
      /*btns.setMaxSize(260,260);    // 10+80+80+80+10 = 260
        btns.setMinSize(260,260);
        btns.setPrefSize(260,260); */
        // ボタン領域のサイズを固定。余白を含むので実際の内部サイズは(320,320)
        btns.setMaxSize(340,340);    // 10+80+80+80+80+10 = 340
        btns.setMinSize(340,340);
        btns.setPrefSize(340,340);
        
        for (int i = 0; i < bname.length; i++) {
            Button btn = new Button(bname[i]); // ボタンの生成
            // 個々のボタンのサイズを(80,80)に固定 (80x3=240)
            btn.setMaxSize(80,80);  
            btn.setMinSize(80,80);
            btn.setPrefSize(80,80);
            // ボタンが押下された時，計算担当のcalcに渡して処理させる
            btn.setOnAction(event->calc.push(btn.getText()));
            // グリッド上の位置を指定しての配置
            btns.add(btn, i%cols, i/cols); 
        }
        
        // VBOXレイアウトを使い、上に結果エリア，下にボタンを配置
        VBox root = new VBox();
        root.setAlignment(Pos.CENTER); // 配置を中央揃えに指定
        root.setPadding(new Insets(10,10,10,10));
        root.setSpacing(10.0); // 上下の間隔を10に指定
        root.getChildren().addAll(win,btns); // レイアウトに配置
        
        // コントロールのデザインをCSSを使って定義(CSSについては次週説明予定)
        String style = getClass().getResource("mycalc.css").toExternalForm();
        Scene scene = new Scene(root);
        scene.getStylesheets().add(style);
        
        // シーンの割当てと表示
        stage.setScene(scene);
        stage.show();                     
    }

    //-------------------------------------------------------------------------
    /**
     * JavaFXアプリケーションを起動するだけのメインメソッド
     * ※基本的にはこの内容で固定と考えてよい
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);  // JavaFXアプリケーションを起動する
    }
}
