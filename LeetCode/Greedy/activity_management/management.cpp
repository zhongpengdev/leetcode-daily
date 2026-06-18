#include<vector>
#include<algorithm>

struct Activity {
    int start;
    int finish;
    int id;
};

bool compare(Activity a, Activity b) {
    return a.finish < b.finish;
}

std::vector<int> activitySelection(std::vector<Activity> &activites) {
    int n = activites.size();
    std::vector<int> selected;

    std::sort(activites.begin(), activites.end(), compare); //按照结束时间升序排列

    selected.push_back(activites[0].id);
    int lastFinished = activites[0].finish;

    for(int i=0; i<n; i++) {
        if(activites[i].start >= lastFinished) {
            selected.push_back(activites[i].id);
            lastFinished = activites[i].finish;
        }
    }

    return selected;
}