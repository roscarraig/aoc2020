use std::vec::Vec;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;


fn main() {
    let mut numbers: Vec<i32> = Vec::new();
    if let Ok(lines) = read_lines("day1.txt")
    {
        for line in lines
        {
            if let Ok(num) = line
            {
                numbers.push(num.parse().unwrap());
            }
        }
    }
    numbers.sort();
    println!("Numbers: {:?}", numbers);
    find_match(numbers, 2020);
}

fn find_match(v: Vec<i32>, t: i32) -> ()
{
    println!("{}", v.len());
    let l = v.len();
    for i in 0..(l - 1)
    {
        for j in 1..(l - i)
        {
            if v[i] + v[l - j] == t
            {
                println!("x {}", v[i]);
                println!("y {}", v[l - j]);
                println!("{}", v[i] * v[l - j]);
            }
        }
    }
}

// The output is wrapped in a Result to allow matching on errors
// Returns an Iterator to the Reader of the lines of the file.
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
