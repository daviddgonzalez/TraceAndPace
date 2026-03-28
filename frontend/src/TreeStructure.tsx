// michael changed the name and file type. David: you should try and maintain the design patterns of the project
// I'm sorry Michael :( I will try harder next time

export interface keyValPair {
  key: number;
  value: string;
}


export interface InceptaNode {
  keyValPairs: keyValPair[];
  condensed: boolean;
  totalNumOfNodes: number;
  displayedNodes: number;
  children?: InceptaNode[];
}

// Summary about what I learned about interfaces in javascript
// It's essentially a struct that doesnt alter how memory is saved
// It just helps us the programmers while making it because when turned into js
// The interface will be deleted. It's just so we/IDE knows what each node/tree has
