# UEFI_SecureBootMode

UEFI セキュアブートのモード表示を行う UEFI アプリケーションです。表示可能なモードは以下の通りです。
ただし、セキュアブート有効時の起動に対応していないため、限定的な条件下での表示になります。

* SetupMode
* UserMode
* AuditMode
* DeployedMode

UEFI セキュアブート未対応の環境では unknown mode と表示されます。

## 開発環境

* Visual Studio Community 2013  
http://www.visualstudio.com/ja-jp/products/visual-studio-community-vs.aspx
* Windows 8.1 Update 64 ビット版
* 64 ビット UEFI 対応 PC
* USB メモリ

## 実行方法

* USB メモリを PC に差し込みます。
* USB メモリを FAT32 形式でフォーマットします。
* efi フォルダを USB メモリにコピーします。
* Windows を再起動します。
* UEFI BIOS 画面を表示させ、USB メモリを Boot 順序最優先に設定します。  
  セキュアブート対応 PC ではセキュアブートを無効に設定します（無効化出来ない PC では動作不可）。
* UEFI BIOS の設定を保存して UEFI BIOS 画面から抜けると UEFI アプリケーションが実行されます。
* USB メモリを PC から抜いて任意のキーを押すと、PC がリセットされて Windows が起動します。

PC の機種によっては、Boot 順序設定変更を恒久的な設定変更ではなく、  
次回起動時のみ有効の設定とした方が好ましい場合があります。恒久的な  
設定変更で、  PC リセット後の Windows 起動がスムーズに行えないか、  
起動しない場合があるためです。ご注意ください。  

## ビルド方法

* UEFI_SecureBootMode.sln をダブルクリックします。
* Visual Studio のセキュリティ警告を回避してプロジェクトを開きます。  
![warning](https://raw.githubusercontent.com/tenpoku1000/UEFI_SecureBootMode/master/images/MSVC.PNG)
* Visual Studio の起動後に F7 キーを押下します。efi フォルダ以下に UEFI アプリケーションが生成されます。

## 謝辞

本ソフトウェアは、以下の外部プロジェクトの成果物を利用しています。感謝いたします。

* gnu-efi http://sourceforge.net/projects/gnu-efi/

## ライセンス

[MIT license](https://raw.githubusercontent.com/tenpoku1000/UEFI_SecureBootMode/master/LICENSE)

## 作者

市川 真一 <suigun1000@gmail.com>

## 参考資料

* UEFI - PhoenixWiki http://wiki.phoenix.com/wiki/index.php/UEFI
* Unified Extensible Firmware Interface Specification Version 2.5 April, 2015
http://www.uefi.org/sites/default/files/resources/UEFI%202_5.pdf
