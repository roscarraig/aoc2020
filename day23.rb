#!/usr/bin/env ruby

def play(input, n, turns)
  prev = 0
  cups = [0] * (n + 1)
  current = input[0].to_i

  for i in 1..(input.length) do
    cups[prev] = {'next': input[i - 1].to_i, 'moving': false} if prev > 0
    prev = input[i - 1].to_i
  end

  if n > input.length
    for i in (input.length + 1)..n do
      cups[prev] = {'next': i, 'moving': false}
      prev = i
    end
  end

  cups[prev] = {'next': current, 'moving': false}

  for i in 1..turns
    c = current
    t = (c - 2) % n + 1

    for j in 1..3 do
      c = cups[c][:next]
      cups[c][:moving] = true
    end

    save = cups[c][:next]
    m = cups[t][:moving]

    while m do
      t = (t - 2) % n + 1
      m = cups[t][:moving]
    end

    cups[c][:next] = cups[t][:next]
    cups[t][:next] = cups[current][:next]
    cups[current][:next] = save
    c = t

    for j in 1..3 do
      c = cups[c][:next]
      cups[c][:moving] = false
    end
    
    current = save
  end

  current = 1

  if n < 10
    output = ''
    for i in 1..n do
      output += current.to_s
      current = cups[current][:next]
    end
    puts "Part 1: #{output}"
  else
    puts "Part 2: #{cups[1][:next] * cups[cups[1][:next]][:next]}"
  end
end

play(ARGV[0], 9, 100)
play(ARGV[0], 1000000, 10000000)
