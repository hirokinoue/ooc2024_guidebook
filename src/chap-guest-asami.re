= デザインパターンを学ぶ

//flushright{
虎の穴ラボ 浅見
//}

はじめまして、虎の穴ラボでエンジニアをしている浅見です。

本記事では、Object-Oriented Conference 2024に寄せて、デザインパターンについて私見を述べていこうと思います。

== はじめに

Javaなどでオブジェクト指向を学習しようと関連記事を漁って行くと、デザインパターンという語にたどり着くと思います。

そこでデザインパターンについて調べようとすると、〇〇パターン、△△パターンといった具体例が多数出てきます。
これらをひとつずつ追っていくと、そのたびにソースコードやUMLを読解することになり、

私が初学者だったころは、これらの理解に時間と労力がかかりイマイチ全体感をつかめない状況が続いた印象があります。
そこで初学者だったころの自分自身にデザインパターンについて概要を説明するとしたらこんな感じかなという話を書いてみようと思います。

== デザインパターンとは

まずデザインパターンとは、よく利用されるクラス設計の形に名前を付けたものです。

特に「オブジェクト指向における再利用のためのデザインパターン」という本が有名で、デザインパターンといったらこの本で紹介された23パターンを指すことが多いです。著者たちの通称からGoFデザインパターンと呼ばれています。
昨今はライブラリやフレームワークが充実しているため、自分でこれらのデザインパターンを用いたクラスを実装することは少ない印象ですが、デザインパターンは複雑なクラス設計を一言で言い表せるため、エンジニア同士の会話で意思の疎通がスムーズになったり、既存のライブラリの使い方を名前から容易に想像できるようになるため、学習しておいて損はないです。

=== デザインパターンへの批判など

よくあるデザインパターンへの批判を挙げてみると、「紹介された23パターンは何か明確な基準のもと体系立てて列挙しているわけではなく、重要なパターンとそうでないパターンがある」、「Iteratorパターンは言語仕様に組み込まれているため、いまさら学習する意味はない」「Singletonパターンは不要である」などなどがあります。というのも、そもそもGoF本が書かれたのは1995年で、Javaがリリースされる前でした。そんな時代に提唱されたものですので、現代のプログラミングの現場の実情とはかけ離れたパターンがあったり、昨今ではほぼ見なくなったパターンもあるようです。
実際、著者への2009年のインタビューで、「Singletonは廃止しても良い」とか、「パターンの分類を今ならこうする～」みたいな話が出ていたようです。
こういった時代背景的な話は、そのときの言語仕様やパラダイム、流行しているフレームワークなどによって今後も変わる可能性があるので、今は何がベストプラクティスなのかということは意識して調べながら学習するのがよさそうです。

ちなみに、Iteratorパターンが学習する価値が無いのか？と聞かれたら、学ぶ価値アリと答えます。
たしかに、今日のJavaの世界では標準ライブラリにIteratorインターフェースが定義されているほか、拡張forなどで言語仕様レベルでIteratorの呼び出し部分が組み込まれているため、巷でみられるIteratorパターンのサンプルコード全体を自分で実装することはまずないです。一方で、Iteratorパターンに理解があれば、拡張for分に独自のクラスを渡したり、Stream#iterateメソッドを使うときにすぐにイメージが湧いたり...といった恩恵を受けれることでしょう。

=== その他のデザインパターン

ちなみに、GoFが最初に提唱した23パターン以外にも、世の中には色々なデザインパターンがあります。

たとえばNullオブジェクトパターンなんてものが典型的です。
また、例えばBuilderパターンについても、GoFで提唱された形、EffectiveJavaで提唱された形など、目的は同じでも実装方法にバリエーションがあるケースもあります。

== デザインパターンを学習する上で大事なこと

私が初学者の時、各パターンのサンプルコードやUMLは理解したが、使い時がよくわからず結局身につかない...という状況に陥った記憶があります。

これはおそらく、当時私がデザインパターンを「これさえやっておけば良い最強の設計！」という銀の弾丸のようなイメージを持っていたからだと思います。
実際には、まず始めに解決したい問題があり、それを解決する手法としてパターンが出てくるので、前提となる問題を抑えておくことが大事です。
デザインパターンを学習すると、早くパターンを使ってみたい！となりがちですが、前提の問題部分をすっ飛ばして無理やりコードを書き始めると、必要以上に複雑なコードを生み出す結果に陥りそうです。(そしてYAGNIと突っ込まれる...)

GoFデザインパターンは本のタイトルにもあるように「再利用性」を課題にしており、特に委譲・コンポジションを駆使し、ポリモーフィズムによってコードの再利用を実現するようなパターンが多い印象です。「再利用性」が問題意識として上がってくるのはそれなりの規模の開発現場になるため、適当なサンプルコードだとデザインパターンを使った書き方がむしろ冗長な書き方に見えて、必要性がわからなくなったりします。このため、コードを写経してもあまり理解が深まらない...なんてことも考えられます。
理想を言うと、実際にそれなりの規模の開発に取り組んで、機能追加やリファクタリングのタイミングなどで「このパターンを使うとコードを再利用できる」と気付ければ、デザインパターンがスッと理解できるように思います。

== さいごに

「デザインパターンとは」の部分で、デザインパターンはクラス設計の典型例に名前が付くことで、意思の疎通がスムーズになるメリットがあると述べました。
そしてこの「名前が付く」という部分が大きなポイントだと感じています。

最近では、GitHub CopilotなどのAIによるアシストのあるコーディング環境というのが一般的になってきました。
こうした環境でAIによるコード補完の恩恵を最大化するためにも、一般に受け入れられる命名を意識することがより一層重要になってきています。
このため、デザインパターンの語を知っておくことは今後も大いに意義があるのではないでしょうか。


　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/kawaiiseeker.png}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        あさみ　@kawaiiseeker https://x.com/kawaiiseeker\\
    \end{minipage}
    \hspace{1ex}
//}
虎の穴ラボでエンジニアをしています。Javaが推し言語です。