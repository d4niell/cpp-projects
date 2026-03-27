using System.Data.SqlTypes;

namespace BlazorApp1
{
    public class Todoitem
    {
        public string? Title { get; set; }
        public bool IsDone { get; set; } = false;
    }
}
