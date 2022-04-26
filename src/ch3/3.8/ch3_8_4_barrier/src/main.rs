use std::sync::{Arc, Barrier};
use std::thread;

fn main() {
    // 後でjoinを行うためにスレッドハンドラを保存するベクタ
    let mut v = Vec::new();

    // 10スレッド分のバリア同期をArcで包む
    let barrier = Arc::new(Barrier::new(10));

    // スレッド起動
    for i in 0..10 {
        let b = barrier.clone();
        let th = thread::spawn(move || {
            b.wait(); // バリア同期
            println!("id {} : finished barrier", i);
        });
        v.push(th);
    }

    for th in v {
        th.join().unwrap();
    }
}
