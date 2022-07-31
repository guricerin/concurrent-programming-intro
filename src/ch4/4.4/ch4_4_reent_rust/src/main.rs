use std::sync::{Arc, Mutex};

fn main() {
    let lock0 = Arc::new(Mutex::new(0));
    // Arcのcloneは参照カウンタを増やすだけ
    let lock1 = lock0.clone();

    let a = lock0.lock().unwrap();
    let b = lock1.lock().unwrap(); // 同じMutexに対してロックするのでデッドロック
    println!("{}", a);
    println!("{}", b);

    // Rustの場合は、他のスレッドに共有リソースを渡す場合にのみcloneし、￥
    // 同一スレッド内ではcloneして利用しない、ということを守ればよい。
}
