use std::sync::RwLock;

fn main() {
    let lock = RwLock::new(10);
    {
        // immutableな参照を取得
        let v1 = lock.read().unwrap();
        let v2 = lock.read().unwrap();
        println!("v1 = {}", v1);
        println!("v2 = {}", v2);
    } // スコープを抜けると自動的にReadロックが解放される。

    {
        // mutableな参照を取得
        let mut v = lock.write().unwrap();
        *v = 7;
        println!("v = {}", v);
    } // スコープを抜けると自動的にWriteロックが解放される。
}
