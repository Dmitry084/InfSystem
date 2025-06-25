using System.Text.Json;

namespace BooksAPI
{
    public partial class Form1 : Form
    {
        public class BookItem
        {
            public VolumeInfo volumeInfo { get; set; }
        }
        public class industryIdentifiers
        {
            public string type { get; set; }
            public string identifier { get; set; }
        }
        public class VolumeInfo
        {
            public string title { get; set; }
            public string[] authors { get; set; }
            public string description { get; set; }
            public string publishedDate { get; set; }
            public int pageCount { get; set; }
            public string previewLink { get; set; }
            public industryIdentifiers[] industryIdentifiers { get; set; }

        }
        

        public class OpenLibraryBook
        {
            public string publish_date { get; set; }
            public int? number_of_pages { get; set; }
        }
        
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }
        private async void button1_Click(object sender, EventArgs e)
        {
            string query = textBox1.Text.Trim();
            if (string.IsNullOrEmpty(query)) return;

            flowLayoutPanel1.Controls.Clear();

            var books = await SearchBooksAsync(query);

            foreach (var book in books)
            {
                string isbn = ExtractISBN(book);
                OpenLibraryBook olBook = null;
                if (!string.IsNullOrEmpty(isbn))
                {
                    olBook = await GetOpenLibraryDetailsAsync(isbn);
                }
                if (olBook != null)
                {
                    if (!string.IsNullOrEmpty(olBook.publish_date))
                        book.volumeInfo.publishedDate = olBook.publish_date;

                    if (olBook.number_of_pages.HasValue)
                        book.volumeInfo.pageCount = olBook.number_of_pages.Value;
                }
                var panel = new Panel();
                panel.BorderStyle = BorderStyle.FixedSingle;
                panel.Width = 700;
                panel.Margin = new Padding(10);
                panel.AutoScroll = true;
                panel.Padding = Padding.Empty;
                var title = new Label();
                title.Text = $"Название: {book.volumeInfo.title}";
                title.AutoSize = true;
                title.Font = new Font(title.Font, FontStyle.Bold);
                title.MaximumSize = new Size(680, 0);

                var authors = new Label();
                authors.Text = $"Автор(ы): {string.Join(", ", book.volumeInfo.authors ?? new string[] { "Неизвестно" })}";
                authors.AutoSize = true;
                authors.MaximumSize = new Size(680, 0);

                var description = new Label();
                description.Text = $"Описание: {book.volumeInfo.description ?? "Нет описания"}";
                description.AutoSize = true;
                description.MaximumSize = new Size(680, 0);
                description.TextAlign = ContentAlignment.MiddleLeft;

                var publishYear = new Label();
                publishYear.Text = $"Год издания: {book.volumeInfo.publishedDate?.Split('-')[0] ?? "Неизвестно"}";
                publishYear.AutoSize = true;

                var pages = new Label();
                pages.Text = $"Количество страниц: {(book.volumeInfo.pageCount > 0 ? book.volumeInfo.pageCount.ToString() : "Неизвестно")}"; 
                pages.AutoSize = true;

                var link = new LinkLabel();
                link.Text = "Ссылка на книгу";
                link.Links.Add(0, link.Text.Length, book.volumeInfo.previewLink);
                link.LinkClicked += (s, args) =>
                {
                    if (!string.IsNullOrEmpty(book.volumeInfo.previewLink))
                    {
                        System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo
                        {
                            FileName = book.volumeInfo.previewLink,
                            UseShellExecute = true
                        });
                    }
                };
                link.AutoSize = true;

                panel.Controls.Add(title);
                panel.Controls.Add(authors);
                panel.Controls.Add(description);
                panel.Controls.Add(publishYear);
                panel.Controls.Add(pages);
                panel.Controls.Add(link);

                int y = 10;
                title.Location = new Point(5, y);
                y += title.Height + 5;

                authors.Location = new Point(5, y);
                y += authors.Height + 5;

                description.Location = new Point(5, y);
                y += description.Height + 5;

                publishYear.Location = new Point(5, y);
                y += publishYear.Height + 5;

                pages.Location = new Point(5, y);
                y += pages.Height + 5;

                link.Location = new Point(5, y);
                panel.Height = y + 30;
                flowLayoutPanel1.Controls.Add(panel);
            }
        }
        private async Task<List<BookItem>> SearchBooksAsync(string query)
        {
            string searchQuery = query.Trim();
            if (string.IsNullOrEmpty(searchQuery)) return new List<BookItem>();

            string encodedQuery = Uri.EscapeDataString(searchQuery);
            string combinedQuery = $"intitle:{encodedQuery}+OR+inauthor:{encodedQuery}";
            string subQuery = $"subject:{encodedQuery}";


            string url = $"https://www.googleapis.com/books/v1/volumes?q={combinedQuery}";
            string url2 = $"https://www.googleapis.com/books/v1/volumes?q={subQuery}";

            using (HttpClient client = new HttpClient())
            {
                try
                {
                    string response = await client.GetStringAsync(url);
                    string response2 = await client.GetStringAsync(url2);
                    var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                    var options2 = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                    var result = JsonSerializer.Deserialize<Dictionary<string, object>>(response);
                    var result2 = JsonSerializer.Deserialize<Dictionary<string, object>>(response);
                    if (result?.ContainsKey("items") == true)
                    {
                        var itemsJson = JsonSerializer.Serialize(result["items"]);
                        if (result2?.ContainsKey("items") == true)
                        {
                            itemsJson = JsonSerializer.Serialize(result2["items"]);
                        }
                        return JsonSerializer.Deserialize<List<BookItem>>(itemsJson, options);
                    }

                    return new List<BookItem>();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Ошибка при загрузке данных: " + ex.Message);
                    return new List<BookItem>();
                }
            }
        }
        private string ExtractISBN(BookItem book)
        {
            if (book?.volumeInfo?.industryIdentifiers != null)
            {
                foreach (var id in book.volumeInfo.industryIdentifiers)
                {
                    if (id.type == "ISBN_10" || id.type == "ISBN_13")
                        return id.identifier;
                }
            }
            return null;
        }
        private async Task<OpenLibraryBook> GetOpenLibraryDetailsAsync(string isbn)
        {
            if (string.IsNullOrEmpty(isbn)) return null;

            string url = $"https://openlibrary.org/api/books?bibkeys=ISBN:{isbn}&format=json&jscmd=data";

            using (HttpClient client = new HttpClient())
            {
                try
                {
                    string response = await client.GetStringAsync(url);
                    var result = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(response);

                    foreach (var item in result)
                    {
                        var book = new OpenLibraryBook();

                        if (item.Value.TryGetProperty("publish_date", out var pubDate))
                            book.publish_date = pubDate.GetString();

                        if (item.Value.TryGetProperty("number_of_pages", out var pages))
                            book.number_of_pages = pages.GetInt32();

                        return book;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Ошибка при получении данных из Open Library: {ex.Message}");
                }
            }

            return null;
        }
    }
}
