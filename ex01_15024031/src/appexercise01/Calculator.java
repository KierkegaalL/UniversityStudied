package appexercise01;

import javafx.scene.control.TextField;

/**
 * オブジェクト指向応用演習　前半課題
 * 電卓プログラムの計算処理クラス
 * @author 15024031 　奥　龍司
 */
public class Calculator {
    // 計算結果表示エリアへの参照(コンストラクタで参照をもらう)
    TextField win;
    // 計算に用いられる第１被演算子(A+BのA)
    long operand1 = 0;   
    // 計算に用いられる第２被演算子(A+BのB)
    long operand2 = 0;   
    // 演算子(+-*/など。A+Bの+)
    String operator = null;  
    
    Boolean input_flag = false;
    
    //-------------------------------------------------------------------------
    /**
     * コンストラクタ
     * @param win 
     */
    Calculator(TextField win){
        // メインクラスが持つテキスト描画エリアへの参照
        this.win = win;
        
        // 各変数の初期化処理をここに書く
         
    }
    
    /**
     * ボタン押下時の電卓処理
     * @param btn 
     */
        void push(String btn){
        // 何も書いていないボタン(ひな型には無い)が押されたら何もしない
        if (btn.equals("")) {
            return; // 以降を処理せずここでメソッド終了
        }

        // ボタンの種類ごとに処理を実行
        switch (btn) {
            case "0": // 左のように並べると0-9のいずれかの場合という処理になる
            case "1": // break; でcase処理が区切られる
            case "2":
            case "3":
            case "4":
            case "5":
            case "6":
            case "7":
            case "8":
            case "9":
                String num =win.getText();
                
                if(num.equals("0")){
                    win.setText(btn);
                    }else if(num.length()<8){
                // 数字ボタン押下時の処理
                        num =win.getText();
                        win.setText(num+btn); // 計算画面に押された数値をそのまま表示
                }
 
                if(input_flag){
                    input_flag =false;
                    win.setText(btn);
                }
                break;
                
            case "AC":  // 全消去
                // ACボタン押下時の処理はここに書く
                operand1 =0;
                operand2 =0;
                operator =null;
                input_flag =false;
                win.setText("0");
                break;
                
            case "C":  // 入力初期化
                // Cボタン押下時の処理はここに書く
                win.setText("0");
                break;
                
            case "+":
            case "-":
            case "*":
            case "/":
                long result;// = calculate(operand1,operand2,operator);
                
                if(operator==null || operator.equals("=")){
                    // 演算子ボタン押下時の処理はここに書く
                    operand1 =Long.parseLong(win.getText());
                }else{
                    operand2 =Long.parseLong(win.getText());
                    operand1 =calculate(operand1,operand2,operator);
                }
                    operator =btn;
                    input_flag =true;
                    win.setText(""+operand1);
                
                    
                break;
                
            case "=":
                // ＝ボタン押下時の処理はここに書く
                operand2 = Long.parseLong(win.getText());
                result =calculate(operand1,operand2,operator);
                input_flag =true;
                win.setText(""+result);
                operand1 = result;
                operator = null;
                
                break;

            // 他のボタンがあるならばここ以下に同様にcase文を追加していく
            case "%":
                
                break;
                
            case "√":

                break;
        }

        //==================================
        // ボタン押下の後処理はこの辺に追加する(計算結果の書き戻しなど)
        
        
        
        
        //==================================
    }
    
    //-------------------------------------------------------------------------
    /**
     * 演算タイミングでの演算処理を実行
     * @return 計算結果
     */
    long calculate(long operand1,long operand2, String operator){
       long result =0;
       System.out.println("(1, 2, 3):("+operand1+", "+operand2+", "+operator+")");
       switch(operator){
           case "+":
               result = operand1 + operand2;
               win.setText(""+operand1);
               break;
           case "-":
               result = operand1 - operand2;
               win.setText(""+operand1);
               break;
           case "*":
               result = operand1 * operand2;
               win.setText(""+operand1);
               break;
           case "/":
               result = operand1 / operand2;
               win.setText(""+operand1);
               break;
       }
        return result;
    }
    public void appear(String str){
        long l=Long.parseLong(win.getText());
        
    }
}