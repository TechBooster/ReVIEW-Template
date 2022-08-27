
= 棋譜データから音楽を作ろう

== この記事でやること
データを音楽にする取り組みは過去にも多く行われてきました。目的は様々で、自然の営みを理解しようとするものや、美しい音楽を作ろうとするものまで何でもありです。
ここで重要なのは、データさえあれば音楽を作れるということです。その入り口を理解すれば、どのようなデータであってもルールを課すことで音に変換することができます。
この記事ではデータから音楽を作る流れをご紹介します。今回は視覚的にも面白そうな棋譜データからMIDI生成と動画生成を行いました。

== 手順

=== 棋譜データ(CSA形式)の読み込み
棋譜データにはいくつか種類があります、今回はCSA形式を扱います。将棋DB2@<fn>{endofn02}など公開されている棋譜データを扱いましょう。読み込みライブラリにはgunyarakun/python-shogi@<fn>{endofn03}を使用します。

//list[endo_csa][棋譜データの読み込み][Python]{
import shogi

kif = shogi.CSA.Parser.parse_file('test.csa')[0]
board = shogi.Board()
for move in kif['moves']:
    # どこからどこに移動したか USIプロトコル形式
    print(move)
    # 移動
    board.push_usi(move)

    # 盤面表示
    print(board) # テキスト形式(ASCII文字)
    print(board.kif_str()) # テキスト形式(KIFスタイル)

    # 占有している駒の位置 (右下から左に移動という方向で表記されている)
    print('{:0=81b}'.format(board.occupied[shogi.BLACK]))# 先手
    print('{:0=81b}'.format(board.occupied[shogi.WHITE]))# 後手

    print('-'*100)
//}

駒と位置の表記法はUSIプロトコル@<fn>{endofn04}として定義されています。駒はアルファベット表記で大文字が先手で小文字が後手、盤面表記では1段目の9筋側から表記され、数字が書いてあったらそこから何マス空白があるかを表しています。

=== PythonでMIDI生成準備
PythonでMIDI生成にはmido@<fn>{endofn05}を使いました。たとえば以下のように書くとMIDIが生成できます。このコードではnoteで音の番号を指定しており、ドミソと順番に鳴ります。なおmidoを使い和音を出したりトラック分けしたりも可能です。

//list[endo_midi][pythonでmidi出力][Python]{
import mido
from mido import Message, MidiFile, MidiTrack, MetaMessage

mid = MidiFile()
track = MidiTrack()
mid.tracks.append(track)
track.append(MetaMessage('set_tempo', tempo=mido.bpm2tempo(120)))

track.append(Message('note_on', note=60, velocity=64, time=0))
track.append(Message('note_off', note=60, time=480))
track.append(Message('note_on', note=60+4, velocity=64, time=0))
track.append(Message('note_off', note=60+4, time=480))
track.append(Message('note_on', note=60+7, velocity=64, time=0))
track.append(Message('note_off', note=60+7, time=480))

mid.save('new_song.mid')
//}

=== 棋譜データを音に変換する

データさえあればそれをMIDIに変換できます。変換時のルールが音楽を決めるため重要な部分となりますが、今回は盤面の中央がC4の音になるように変換する単純な例を考えます。

//list[endo_cas_to_midi][pythonで寄付データをmidiに変換][Python]{
import shogi.CSA
import mido
from mido import Message, MidiFile, MidiTrack, MetaMessage

kif = shogi.CSA.Parser.parse_file('test.csa')[0]

mid = MidiFile()
track = MidiTrack()
mid.tracks.append(track)
track.append(MetaMessage('set_tempo', tempo=mido.bpm2tempo(120)))

number = 0
for move in kif['moves']:
    x = int(move[2])
    y = int(ord(move[3].upper()))-64
    node_pos = (x + (y-1)*9) +19 # 19加えることで、81の範囲の中央をC4に指定する

    track.append(Message('note_on', note=node_pos, velocity=127, time=0))
    track.append(Message('note_off', note=node_pos, time=480))
    number = number + 1

mid.save('new_song.mid')
//}


=== 棋譜データを音に変換する(別バージョン)

先ほどのように変換してしまうと音がランダムすぎるので、少し改善したのが以下のように割り当てる方法です。

//indepimage[endo_01][図1 将棋盤と音の割り当て例]

81マスに81音を単純に割り当てると高音や低音が聞き分けられない上に段ごとの区別がつかなくなるため、オクターブごとに1段を割り当てます。同じ音が割り振られることがある代わりに列について音に一貫性が出る配置です。
このように変換ルールはいかようにも作ることができます。


=== MIDIをwavに変換

MIDIは動画と一緒に扱うには扱いづらいためwaveに変換しておきます。pythonでも可能です。ここでは楽器選びなども行うためGarageBandを使いました。メニューの「共有」から「曲をディスクに書き出す」を選択してください。


=== Pythonとffmpegで動画向け連続画像出力
今回はスクリプトから動画生成する方法の中でも難易度が低いパラパラマンガ方式で動画を作成します。Pythonで動画にする連続画像を出力し後でパラパラマンガのように結合することで動画にします。
動画として棋譜から盤面そのものを生成するのも良いでしょう。ここでは盤面を単なる表と捉えてヒートマップとして扱い、グラフ出力の形で画像出力をおこないます。
Matplotlib@<fn>{endofn06}とseaborn@<fn>{endofn07}を使うと以下のように書けます。

//list[endo_python_image][棋譜データの読み込み][Python]{
def outputHeatmap(heatmap_array, vmax, image_no):
    df = pd.DataFrame(data=heatmap_array, index=['一', '二', '三', '四', '五', '六', '七', '八', '九'], columns=['9', '8', '7', '6', '5', '4', '3', '2', '1'])

    plt.figure(dpi=80, figsize=(16,9))
    sns.set(font='IPAexGothic', font_scale=2.5)
    plt.gca().get_xaxis().set_ticks_position('top')
    plt.gca().get_yaxis().set_ticks_position('right')
    heatmap = sns.heatmap(df, cmap='Oranges', annot=True, square=True, cbar=False,\
        vmin=0, vmax=vmax)
    heatmap.set_yticklabels(heatmap.get_yticklabels(), rotation=0) 
    plt.savefig('animation/heatmap_' + str(image_no).zfill(4) + '.png')
    plt.close('all')
//}

ここに盤面の状態を配列にして流し込むことで画像を得ます。


=== ffmpegで連続画像から動画出力し音声を結合
まず連続する画像から動画を作成します。1秒あたりの枚数に注意しましょう。ffmpeg@<fn>{endofn08}により以下のように書けます。BPMからフレームレートを決めます。

//list[endo_image_to_movie][画像から動画]{
1秒に1画像を使う場合
./ffmpeg -framerate 1 -i %04d.png -vcodec libx264 -pix_fmt yuv420p -r 30 out.mp4
1秒に2画像を使う場合
./ffmpeg -framerate 2 -i %04d.png -vcodec libx264 -pix_fmt yuv420p -r 30 out.mp4
//}

出力した動画と画像を結合します。

//list[endo_merge_sound_and_movie][動画と音声を結合]{
./ffmpeg -i out.mp4 -i out.wav result.mp4
//}

=== 完成
完成です！早速再生して確認しましょう。

//indepimage[endo_02][図2 https://www.youtube.com/watch?v=x8j75QkE_x0]

ここまでやってきて残念ながら音楽を生成する場合はデータを音に変換するルールがしっかりしていないととても聞ける音にはなりません。
予測できない将棋盤を音に置き換えて旋律が浮かび上がってくることは稀で、かつ今回のようなシンプルな置換ルールではこの辺りが限界のようです。


=== 改良
ですがここから改良していけば良いものが完成するかもしれません。
例えば、音を変換することを諦めて、盤面をリズムとして捉えて変化した動画がこちらです。

//indepimage[endo_03][図3 https://www.youtube.com/watch?v=OhnLqbTmCF8]

ある瞬間の棋譜について、3,5,7行目だけに着目し、1-9列について順番に見て駒があるなら音を鳴らし、駒がないなら音を出さないというルールで変換しています。
このように同じデータから音へ変換する方法は様々あります。

== まとめ
データがあれば、音も動画も作ることが可能です。ぜひチャレンジしてみてください。

//footnote[endofn02][https://shogidb2.com/]
//footnote[endofn03][https://github.com/gunyarakun/python-shogi]
//footnote[endofn04][http://shogidokoro.starfree.jp/usi.html]
//footnote[endofn05][https://github.com/mido/mido]
//footnote[endofn06][https://matplotlib.org/]
//footnote[endofn07][https://seaborn.pydata.org/]
//footnote[endofn08][https://ffmpeg.org/]

