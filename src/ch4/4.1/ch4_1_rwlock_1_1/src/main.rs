use std::sync::{Arc, RwLock};
use std::thread;

fn main() {
    let val = Arc::new(RwLock::new(true));

    let t = thread::spawn(move || {
        let flag = val.read().unwrap();
        if *flag {
            // Readロック獲得中にWriteロックを獲得するので、デッドロックが発生。
            *val.write().unwrap() = true;
            println!("flag is true");
        }
    });

    t.join().unwrap();
}
