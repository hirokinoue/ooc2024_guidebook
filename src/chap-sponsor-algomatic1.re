= 生成AIの不確実性に立ち向かうソフトウェアアーキテクチャ

//flushright{
Algomatic シゴラクAI カンパニーCTO takuya kikuchi @_pochi
//}

//note{
当稿は生成AI Conf「実践LLMエンジニアリング」での登壇内容を、
Object-Oriented Conference 2024ガイドブック向けに加筆・修正したものです

https://speakerdeck.com/tkikuchi1002/llm-engineering-architecture
//}

AlgomaticのシゴラクAIカンパニーでは、生成AIを活用した生産性向上支援SaaSである「シゴラクAI」というプロダクトを開発・提供しています。
本稿では、生成AIを組み込んだプロダクトを開発していく中での設計の工夫について書いていきます。

== 生成AIとは

そもそも生成AI（Generative Artificial Intelligence）とは、機械学習の手法を利用して、既存のデータセットを基に新しい情報やパターンを創出するAI技術です。これらは、トレーニングされたデータから学習した構造を利用して、テキスト、音声、画像などの新しい出力を生成する能力を持っています。
わたしたちソフトウェアエンジニアとしては、ChatGPTにコーディングの相談をしたり、GitHub CopilotやCopilot Chatなどを通じて触れることが多いかもしれません。

生成AIが世間に認知されるようになったのはここ数年の出来事であり、GitHub Copilotは2021年、ChatGPTは2022年末にリリースされました。
さらに2023年初頭にはChatGPTでGPT-4というモデルが利用可能となり、その性能の高さが大きな話題となったことは記憶に新しいです。

生成AIの利用シーンは大変幅広いのですが、わたしとしては「非構造化データを構造化データに変換することが非常に得意」だという点が非常に気に入っています。

//embed[latex]{
\clearpage
//}

//emlist[]{
電気通信大学在学中から、セキュリティソリューションを手がけるスタートアップにてソフトウェアエンジニアとしてインターンシップに従事。その後複数のスタートアップ企業を経て、フィンテック系スタートアップでVPoEに就任。その後、飲食店向けSaaSプロダクトを手掛ける株式会社Showcase GigではVPoTとして技術統括および開発組織運営を行う。2023年8月にAlgomaticにシゴラクAI事業部CTOとして参画
//}

上記は私のプロフィール文章ですが、これを生成AIを使って構造化データに変換したのが以下のJSONです。

//emlist[]{
{
  "出身大学": "電気通信大学",
  "経験した職種": [
    "ソフトウェアエンジニア",
    "VPoE（Vice President of Engineering）",
    "VPoT（Vice President of Technology）",
    "CTO（Chief Technology Officer）"
  ],
  "現在の肩書き": "シゴラクAI事業部CTO"
}
//}

このような自然言語から構造化データ変換を、事前のモデル準備なども必要なく簡単なプロンプト1つで行えてしまうのです。
生成AIによって、これまで諦めていたようなプロダクトが、あれもこれも作れるのではないか...と、ワクワクしませんか？

== 生成AIをプロダクトで活用するということ

そんな生成AIをプロダクトに組み込む際にわたしたちソフトウェアエンジニアが頭を悩ます要素として、代表的なものは「利用コスト」「回答速度」「コンテキスト長」などがあります。
利用コストは生成AIを呼び出す際のAPI利用料であり、回答速度はそのAPIによるレスポンスが全て得られるまでの時間です。
コンテキスト長は、「生成AIに一度に渡せる文字列の長さ」のことで、その長さに一定の制限があるのが現状です。

それらの問題に対して、以下のような創意工夫をもってプロダクトに落とし込んでいきます。

 * 利用コスト: コストを下げるために安価なモデルを利用する、自前でLLMを構築する、
 * 回答速度: キャッシュ処理を工夫したり、遅さが気にならないように体験を工夫したり
 * コンテキスト長: テキストを分割したり、要約をしたり

また、生成AIのモデルによってこれらの特徴は大きく異なります。現時点の自然言語の取り扱いにおいて精度が最も高いのはGPT-4というモデルですが、
このモデルは比較的コストは高く、回答速度は遅めです。ちなみに、その発展系であるGPT-4-Turboはかなり回答速度は早くなりましたが、GPT-3.5-Turboに比べるとまだまだ遅さが感じられます。


== そんな生成AIを活用するプロダクトをどう設計するか

AlgomaticのシゴラクAIカンパニーで開発・運用しているシゴラクAIは、いわゆる「法人向けChatGPT」と呼ばれるカテゴリの製品です。
顧客への提供価値の中心に「生成AIそのもの」を据えたサービスであり、生成AIなしには成立しないプロダクトです。

//indepimage[shigoraku][シゴラクAIのスクリーンショット][scale=0.5]

どういうプロダクトかといえば、本家ChatGPTを思い浮かべてもらえればわかりやすいです。「チャットUIでAIに話しかけると返事がもらえる」というシンプルなものです。
みなさんなら、どのような設計をしますか？今後の機能拡充を見越して、クリーンアーキテクチャのようにレイヤーを分けるでしょうか。それとも、階層構造は設けず、フラットな構成とする...あるいは、さらに異なる考え方で設計するでしょうか。

//indepimage[architecture1][初期アーキテクチャ][scale=0.7]

シゴラクAIの最初期は、抽象化なども挟まず、レイヤーの定義などもしない非常にシンプルな設計でした。

少人数で開発していましたし、事業としての不確実性も高く、クリーンアーキテクチャのような今後のスケールを見越した手厚い設計は適さないと判断しました。
当然機能としてもそれほど多くなく、ユーザーがログインできて、生成AIに話しかけることができて、その返事をもらえる。それ以外には、ユーザー管理やコスト管理、請求管理の機能がある、というくらいでした。

ただしその後、事業が進むにつれて「社内ドキュメントに基づいて回答をする機能」、「Webページの内容を自動で取得して回答する機能」といったAIによる回答アルゴリズムを拡張するような機能開発や、
「Slackで問いかけられる機能」、UIリニューアルなど、ユーザー体験を新たにするような改修などを行うことになります。
その過程で「これは何か秩序を設けないとまずいな」となり、シゴラクAIに必要なアーキテクチャ上の観点を考えました。

 1. コアドメインを互いに分離したい
 2. 生成AIのモデルを切り替え可能にしたい

図にするとこのようになります。

//indepimage[architecture2][現在のアーキテクチャ][scale=1.0]

CharEngineとLLM Gatewayというインターフェースを用意し、抽象化を行なっています。
それ以外にレイヤー定義などは行わず「必要な箇所に、必要な抽象化のみを行なった」状態としました。それぞれの観点について解説します。

== コアドメインを互いに分離する

1つ目の観点は「コアドメインを互いに分離する」ことです。
シゴラクAIの価値は、UIも含めた「ユーザーとAIアシスタントとの対話」の体験です。
コアドメインは「ユーザー体験」と「AIによる回答品質」であり、これらの品質を高めていくことが大切だと考えています。

そのために、UIは柔軟に変更し、価値検証を繰り返せるようにしたい。裏側のロジックや事情に左右されることは避けたい。
一方で、AIの回答品質はUIに関係なく柔軟に開発をしたい。
さらに、「社内ドキュメントに基づく回答をする」、「Webページの内容に基づいて回答する」など、ユースケースごとにAIの回答アルゴリズムも複数存在できる必要がありました。

//indepimage[core][コアドメインを互いに分離する][scale=0.7]

それらを実現するため、AIとユーザーのやり取りを「ChatEngine」として抽象化し、UIと各エンジンは互いに依存せず、抽象にのみ依存する設計としました。
UIはChatEngineのインターフェースのみ意識して開発すればよく、回答アルゴリズムも、ChatEngineのインターフェースに適合してさえいれば、裏側では自由に開発できます。
これにより、「試験的に全く新しい回答アルゴリズムを試す」といったことも容易に可能になりました。

== 生成AIのモデルを切り替え可能にする

2つ目の観点は「生成AIのモデルを切り替え可能にする」ことです。経験的に、特定の生成AIサービスやモデルに依存することは避け、柔軟な切り替えを可能とすることの価値が高いと考えています。

新しいモデルが突然登場したり、過去の制約条件が突然取っ払われたりするなどのポジティブな変化に素早く対応できることが事業価値につながりますし、
反対に、突然特定のモデルが利用不能になった際の事業リスクを緩和したい、という意図もあります。

また少し具体的な話になってしまいますが、呼び出し先がAzure OpenAI Serviceであれば、Quota制限緩和のために複数エンドポイントの呼び分けをする必要があったり、「生成AIの回答をJSON形式に固定する」ような、特定のモデルにしか存在しない機能などもあり、これらの「生成AIの呼び出しだけに関わる事情」を外に染み出させたくない。さらに、コストや速度面を意識したチューニングにおいては、呼び出し先のモデルを切り替えて動作テストを行いたくなることもあります。そこで、LLMの呼び出し層を「LLM Gateway」として抽象化しました。

//indepimage[llm_gateway][生成AIのモデルを切り替え可能にする][scale=0.7]

== まとめ
以上が、シゴラクAIの現時点での設計、およびその考え方です。繰り返しになりますが、必要な抽象化のみを行なっただけで、クリーンアーキテクチャなどのように明確なレイヤーの定義やルール決めなどはしていません。
ただし、「依存の方向を制限してコアドメインを依存関係の頂点に置く」という考え方はクリーンアーキテクチャと同様であり、部分的にエッセンスとして活用している状態です。
こういった設計は今後も継続的に見直し、改善を続けていくことになります。現時点での正解が半年後も正解とは限りません。

ですから、「これは現状の姿であり、今後変わりゆく可能性が高い」とチーム全体で認識しておくことが何よりも大事だと考えています。
そして、こういった話は生成AIに限らず、すべてのプロダクトに通じるところです。

このシゴラクAIの事例がどなたかの役に立てば幸いです。

　

　

//embed{
    \begin{minipage}{.1\linewidth}
        \centering
        \includegraphics[width=.75\linewidth]{images/contributors/algomatic-kikuchi.png}
    \end{minipage}
    \begin{minipage}{.89\linewidth}
        takuya-kikuchi　@\_pochi https://twitter.com/\_pochi\\
        株式会社AlgomaticでシゴラクAIカンパニーCTOを務めています。型とドット絵が好きです。
    \end{minipage}
    \hspace{1ex}
//}