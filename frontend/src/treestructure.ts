export interface InceptaNode {
    key: number;
    value: string;
    nodeDepth: number;
    totalNumOfNodes: number;
    localChildCount: number;
    condensed: boolean;
}

export interface InceptaTree {
    totalNodes: number;
    displayToTotRatio: number;
    visibleNodes: InceptaNode[];
}

//Summary about what I learned about interfaces in javascript
// It's essentially a struct that doesnt alter how memory is saved
// It just helps us the programmers while making it because when turned into js
// The interface will be deleted. It's just so we/IDE knows what each node/tree has