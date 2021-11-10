use std::env;
use std::vec::Vec;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() {
    let args: Vec<String> = env::args().collect();

    if let Ok(lines) = read_lines(&args[1])
    {
        for line in lines
        {
            if let Ok(res) = line
            {
                println!("{}", res);
            }
        }
    }
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
